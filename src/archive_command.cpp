/**
 *
 *  @file archive_command.cpp
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
#include <cgride/toolchains/archive_command.hpp>

#include <string>

#include <cgride/core/error.hpp>
#include <cgride/toolchains/compiler_kind.hpp>

namespace cgride::toolchains
{
  namespace
  {
    using cgride::core::Error;
    using cgride::core::ErrorCode;

    [[nodiscard]] bool archive_options_are_valid(const ArchiveCommandOptions &options) noexcept
    {
      if (options.output.empty() || options.objects.empty())
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

    [[nodiscard]] std::string gnu_archive_flags(const ArchiveCommandOptions &options)
    {
      std::string flags;

      if (options.replace)
      {
        flags += "r";
      }

      if (options.create)
      {
        flags += "c";
      }

      if (options.index)
      {
        flags += "s";
      }

      if (flags.empty())
      {
        flags = "rcs";
      }

      return flags;
    }

    void append_gnu_like_archive_arguments(
        cgride::core::Command &command,
        const ArchiveCommandOptions &options)
    {
      command.arg(gnu_archive_flags(options));
      command.arg(options.output.string());

      for (const auto &object : options.objects)
      {
        command.arg(object.string());
      }
    }

    void append_msvc_like_archive_arguments(
        cgride::core::Command &command,
        const ArchiveCommandOptions &options)
    {
      command.arg("/nologo");
      command.arg("/OUT:" + options.output.string());

      for (const auto &object : options.objects)
      {
        command.arg(object.string());
      }
    }

  } // namespace

  cgride::core::Result<cgride::core::Command> make_archive_command(
      const Toolchain &toolchain,
      const ArchiveCommandOptions &options)
  {
    if (!toolchain.valid())
    {
      return Error(
          ErrorCode::InvalidArgument,
          "Cannot create archive command from an invalid toolchain.");
    }

    if (!archive_options_are_valid(options))
    {
      return Error(
          ErrorCode::InvalidArgument,
          "Cannot create archive command from invalid archive options.");
    }

    if (!toolchain.has_archiver())
    {
      return Error(
          ErrorCode::NotFound,
          "Archiver executable was not found.");
    }

    cgride::core::Command command;

    command.program(toolchain.archiver().value().string());
    command.search_in_path(true);

    if (is_msvc_like(toolchain.compiler_kind()))
    {
      append_msvc_like_archive_arguments(command, options);
    }
    else
    {
      append_gnu_like_archive_arguments(command, options);
    }

    return command;
  }

} // namespace cgride::toolchains
