/**
 *
 *  @file link_command.cpp
 *  @author Gaspard Kirira
 *
 *  Copyright 2026, Gaspard Kirira.
 *  All rights reserved.
 *  https://github.com/cgride/toolchains
 *
 *  Use of this source code is governed by a MIT license
 *  that can be found in the LICENSE file.
 *
 *  Cgride
 *
 */
#include <cgride/toolchains/link_command.hpp>

#include <optional>
#include <string>

#include <cgride/core/error.hpp>
#include <cgride/toolchains/compiler_kind.hpp>

namespace cgride::toolchains
{
  namespace
  {
    using cgride::core::Error;
    using cgride::core::ErrorCode;

    [[nodiscard]] bool link_options_are_valid(const LinkCommandOptions &options) noexcept
    {
      if (options.output.empty() || options.objects.empty())
      {
        return false;
      }

      if (options.artifact_kind != ArtifactKind::Executable &&
          options.artifact_kind != ArtifactKind::SharedLibrary)
      {
        return false;
      }

      for (const auto &object : options.objects)
      {
        if (object.empty())
        {
          return false;
        }
      }

      return true;
    }

    [[nodiscard]] std::optional<std::filesystem::path> linker_program_for(
        const Toolchain &toolchain)
    {
      if (toolchain.has_linker())
      {
        return toolchain.linker().value();
      }

      if (toolchain.has_cxx_compiler())
      {
        return toolchain.cxx_compiler().value();
      }

      return std::nullopt;
    }

    [[nodiscard]] bool library_should_be_passed_directly(const std::string &library)
    {
      const auto path = std::filesystem::path(library);

      return library.starts_with("-") ||
             path.has_parent_path() ||
             path.has_extension();
    }

    void append_gnu_like_link_arguments(
        cgride::core::Command &command,
        const Toolchain &toolchain,
        const LinkCommandOptions &options)
    {
      if (options.artifact_kind == ArtifactKind::SharedLibrary || options.shared)
      {
        command.arg("-shared");
      }

      command.arg("-o");
      command.arg(options.output.string());

      if (toolchain.sysroot().has_value() && !toolchain.sysroot()->empty())
      {
        command.arg("--sysroot=" + toolchain.sysroot()->string());
      }

      if (!toolchain.target_triple().empty())
      {
        command.arg("--target=" + toolchain.target_triple());
      }

      for (const auto &object : options.objects)
      {
        command.arg(object.string());
      }

      for (const auto &directory : toolchain.default_library_directories())
      {
        if (!directory.empty())
        {
          command.arg("-L" + directory.string());
        }
      }

      for (const auto &directory : options.library_directories)
      {
        if (!directory.empty())
        {
          command.arg("-L" + directory.string());
        }
      }

      for (const auto &option : toolchain.builtin_link_options())
      {
        if (!option.empty())
        {
          command.arg(option);
        }
      }

      for (const auto &option : options.options)
      {
        if (!option.empty())
        {
          command.arg(option);
        }
      }

      if (options.strip)
      {
        command.arg("-s");
      }

      for (const auto &library : options.libraries)
      {
        if (library.empty())
        {
          continue;
        }

        if (library_should_be_passed_directly(library))
        {
          command.arg(library);
        }
        else
        {
          command.arg("-l" + library);
        }
      }
    }

    void append_msvc_like_link_arguments(
        cgride::core::Command &command,
        const Toolchain &toolchain,
        const LinkCommandOptions &options)
    {
      command.arg("/nologo");
      command.arg("/OUT:" + options.output.string());

      if (options.artifact_kind == ArtifactKind::SharedLibrary || options.shared)
      {
        command.arg("/DLL");
      }

      for (const auto &object : options.objects)
      {
        command.arg(object.string());
      }

      for (const auto &directory : toolchain.default_library_directories())
      {
        if (!directory.empty())
        {
          command.arg("/LIBPATH:" + directory.string());
        }
      }

      for (const auto &directory : options.library_directories)
      {
        if (!directory.empty())
        {
          command.arg("/LIBPATH:" + directory.string());
        }
      }

      for (const auto &option : toolchain.builtin_link_options())
      {
        if (!option.empty())
        {
          command.arg(option);
        }
      }

      for (const auto &option : options.options)
      {
        if (!option.empty())
        {
          command.arg(option);
        }
      }

      for (const auto &library : options.libraries)
      {
        if (!library.empty())
        {
          command.arg(library);
        }
      }
    }

  } // namespace

  cgride::core::Result<cgride::core::Command> make_link_command(
      const Toolchain &toolchain,
      const LinkCommandOptions &options)
  {
    if (!toolchain.valid())
    {
      return Error(
          ErrorCode::InvalidArgument,
          "Cannot create link command from an invalid toolchain.");
    }

    if (!link_options_are_valid(options))
    {
      return Error(
          ErrorCode::InvalidArgument,
          "Cannot create link command from invalid link options.");
    }

    auto program = linker_program_for(toolchain);

    if (!program.has_value() || program->empty())
    {
      return Error(
          ErrorCode::NotFound,
          "Linker executable was not found.");
    }

    cgride::core::Command command;

    command.program(program.value());
    command.search_in_path(true);

    if (is_msvc_like(toolchain.compiler_kind()))
    {
      append_msvc_like_link_arguments(command, toolchain, options);
    }
    else
    {
      append_gnu_like_link_arguments(command, toolchain, options);
    }

    return command;
  }

} // namespace cgride::toolchains
