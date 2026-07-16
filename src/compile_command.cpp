/**
 *
 *  @file compile_command.cpp
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
#include <cgride/toolchains/compile_command.hpp>

#include <utility>

#include <cgride/core/error.hpp>
#include <cgride/toolchains/compiler_kind.hpp>

namespace cgride::toolchains
{
  namespace
  {
    using cgride::core::Error;
    using cgride::core::ErrorCode;

    [[nodiscard]] bool compile_options_are_valid(const CompileCommandOptions &options) noexcept
    {
      return is_known(options.language) &&
             !options.source.empty() &&
             !options.object.empty();
    }

    void append_gnu_like_compile_arguments(
        cgride::core::Command &command,
        const Toolchain &toolchain,
        const CompileCommandOptions &options)
    {
      command.arg("-c");
      command.arg(options.source.string());
      command.arg("-o");
      command.arg(options.object.string());

      if (options.language == Language::Cxx && !options.cxx_standard.empty())
      {
        command.arg("-std=" + options.cxx_standard);
      }

      if (options.debug)
      {
        command.arg("-g");
      }

      if (options.optimize)
      {
        command.arg("-O2");
      }
      else
      {
        command.arg("-O0");
      }

      if (options.warnings_as_errors)
      {
        command.arg("-Werror");
      }

      if (toolchain.sysroot().has_value() && !toolchain.sysroot()->empty())
      {
        command.arg("--sysroot=" + toolchain.sysroot()->string());
      }

      if (!toolchain.target_triple().empty())
      {
        command.arg("--target=" + toolchain.target_triple());
      }

      for (const auto &directory : toolchain.default_include_directories())
      {
        if (!directory.empty())
        {
          command.arg("-I" + directory.string());
        }
      }

      for (const auto &directory : options.include_directories)
      {
        if (!directory.empty())
        {
          command.arg("-I" + directory.string());
        }
      }

      for (const auto &definition : options.definitions)
      {
        if (!definition.empty())
        {
          command.arg("-D" + definition);
        }
      }

      for (const auto &option : toolchain.builtin_compile_options())
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
    }

    void append_msvc_like_compile_arguments(
        cgride::core::Command &command,
        const Toolchain &toolchain,
        const CompileCommandOptions &options)
    {
      command.arg("/nologo");
      command.arg("/c");
      command.arg(options.source.string());
      command.arg("/Fo" + options.object.string());

      if (options.language == Language::Cxx && !options.cxx_standard.empty())
      {
        command.arg("/std:" + options.cxx_standard);
      }

      if (options.debug)
      {
        command.arg("/Zi");
      }

      if (options.optimize)
      {
        command.arg("/O2");
      }

      if (options.warnings_as_errors)
      {
        command.arg("/WX");
      }

      for (const auto &directory : toolchain.default_include_directories())
      {
        if (!directory.empty())
        {
          command.arg("/I" + directory.string());
        }
      }

      for (const auto &directory : options.include_directories)
      {
        if (!directory.empty())
        {
          command.arg("/I" + directory.string());
        }
      }

      for (const auto &definition : options.definitions)
      {
        if (!definition.empty())
        {
          command.arg("/D" + definition);
        }
      }

      for (const auto &option : toolchain.builtin_compile_options())
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
    }

  } // namespace

  cgride::core::Result<cgride::core::Command> make_compile_command(
      const Toolchain &toolchain,
      const CompileCommandOptions &options)
  {
    if (!toolchain.valid())
    {
      return Error(
          ErrorCode::InvalidArgument,
          "Cannot create compile command from an invalid toolchain.");
    }

    if (!compile_options_are_valid(options))
    {
      return Error(
          ErrorCode::InvalidArgument,
          "Cannot create compile command from invalid compile options.");
    }

    auto compiler = toolchain.compiler_for(options.language);

    if (!compiler.has_value() || compiler->empty())
    {
      return Error(
          ErrorCode::NotFound,
          "Compiler executable was not found for requested language.",
          std::string(to_string(options.language)));
    }

    cgride::core::Command command;

    command.program(compiler->string());
    command.search_in_path(true);

    if (is_msvc_like(toolchain.compiler_kind()))
    {
      append_msvc_like_compile_arguments(command, toolchain, options);
    }
    else
    {
      append_gnu_like_compile_arguments(command, toolchain, options);
    }

    return command;
  }

} // namespace cgride::toolchains
