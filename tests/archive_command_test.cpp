/**
 *
 *  @file archive_command_test.cpp
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
#include <cassert>
#include <filesystem>
#include <string>
#include <vector>

#include <cgride/toolchains/archive_command.hpp>

namespace
{
  [[nodiscard]] bool contains_arg(const std::vector<std::string> &args,
                                  const std::string &value)
  {
    for (const auto &arg : args)
    {
      if (arg == value)
      {
        return true;
      }
    }

    return false;
  }

  [[nodiscard]] cgride::toolchains::Toolchain make_gnu_toolchain()
  {
    cgride::toolchains::Toolchain toolchain(
        cgride::toolchains::CompilerKind::Gcc,
        "GCC");

    toolchain
        .cxx_compiler("/usr/bin/g++")
        .archiver("/usr/bin/ar");

    return toolchain;
  }

  [[nodiscard]] cgride::toolchains::Toolchain make_msvc_toolchain()
  {
    cgride::toolchains::Toolchain toolchain(
        cgride::toolchains::CompilerKind::Msvc,
        "MSVC");

    toolchain
        .cxx_compiler("cl")
        .archiver("lib");

    return toolchain;
  }

} // namespace

int main()
{
  {
    auto toolchain = make_gnu_toolchain();

    cgride::toolchains::ArchiveCommandOptions options;
    options.objects = {
        std::filesystem::path("build/main.o"),
        std::filesystem::path("build/app.o"),
    };
    options.output = "build/libapp.a";

    auto result = cgride::toolchains::make_archive_command(toolchain, options);

    assert(result);

    auto command = result.value();

    assert(command.program() == std::filesystem::path("/usr/bin/ar"));
    assert(command.search_in_path());

    const auto &args = command.args();

    assert(contains_arg(args, "rcs"));
    assert(contains_arg(args, "build/libapp.a"));
    assert(contains_arg(args, "build/main.o"));
    assert(contains_arg(args, "build/app.o"));
  }

  {
    auto toolchain = make_gnu_toolchain();

    cgride::toolchains::ArchiveCommandOptions options;
    options.objects = {
        std::filesystem::path("build/main.o"),
    };
    options.output = "build/libapp.a";
    options.replace = true;
    options.create = false;
    options.index = false;

    auto result = cgride::toolchains::make_archive_command(toolchain, options);

    assert(result);

    auto command = result.value();

    const auto &args = command.args();

    assert(contains_arg(args, "r"));
    assert(contains_arg(args, "build/libapp.a"));
    assert(contains_arg(args, "build/main.o"));
  }

  {
    auto toolchain = make_gnu_toolchain();

    cgride::toolchains::ArchiveCommandOptions options;
    options.objects = {
        std::filesystem::path("build/main.o"),
    };
    options.output = "build/libapp.a";
    options.replace = false;
    options.create = false;
    options.index = false;

    auto result = cgride::toolchains::make_archive_command(toolchain, options);

    assert(result);

    auto command = result.value();

    const auto &args = command.args();

    assert(contains_arg(args, "rcs"));
  }

  {
    auto toolchain = make_msvc_toolchain();

    cgride::toolchains::ArchiveCommandOptions options;
    options.objects = {
        std::filesystem::path("build/main.obj"),
        std::filesystem::path("build/app.obj"),
    };
    options.output = "build/app.lib";

    auto result = cgride::toolchains::make_archive_command(toolchain, options);

    assert(result);

    auto command = result.value();

    assert(command.program() == std::filesystem::path("lib"));
    assert(command.search_in_path());

    const auto &args = command.args();

    assert(contains_arg(args, "/nologo"));
    assert(contains_arg(args, "/OUT:build/app.lib"));
    assert(contains_arg(args, "build/main.obj"));
    assert(contains_arg(args, "build/app.obj"));
  }

  {
    cgride::toolchains::Toolchain toolchain(
        cgride::toolchains::CompilerKind::Gcc,
        "Invalid GCC");

    cgride::toolchains::ArchiveCommandOptions options;
    options.objects = {
        std::filesystem::path("build/main.o"),
    };
    options.output = "build/libapp.a";

    auto result = cgride::toolchains::make_archive_command(toolchain, options);

    assert(!result);
    assert(result.error().code() == cgride::core::ErrorCode::InvalidArgument);
    assert(result.error().message() == "Cannot create archive command from an invalid toolchain.");
  }

  {
    auto toolchain = make_gnu_toolchain();

    cgride::toolchains::ArchiveCommandOptions options;
    options.output = "build/libapp.a";

    auto result = cgride::toolchains::make_archive_command(toolchain, options);

    assert(!result);
    assert(result.error().code() == cgride::core::ErrorCode::InvalidArgument);
    assert(result.error().message() == "Cannot create archive command from invalid archive options.");
  }

  {
    auto toolchain = make_gnu_toolchain();

    cgride::toolchains::ArchiveCommandOptions options;
    options.objects = {
        std::filesystem::path("build/main.o"),
    };
    options.output = std::filesystem::path{};

    auto result = cgride::toolchains::make_archive_command(toolchain, options);

    assert(!result);
    assert(result.error().code() == cgride::core::ErrorCode::InvalidArgument);
    assert(result.error().message() == "Cannot create archive command from invalid archive options.");
  }

  {
    auto toolchain = make_gnu_toolchain();

    cgride::toolchains::ArchiveCommandOptions options;
    options.objects = {
        std::filesystem::path{},
    };
    options.output = "build/libapp.a";

    auto result = cgride::toolchains::make_archive_command(toolchain, options);

    assert(!result);
    assert(result.error().code() == cgride::core::ErrorCode::InvalidArgument);
    assert(result.error().message() == "Cannot create archive command from invalid archive options.");
  }

  {
    cgride::toolchains::Toolchain toolchain(
        cgride::toolchains::CompilerKind::Gcc,
        "GCC");

    toolchain.cxx_compiler("/usr/bin/g++");

    cgride::toolchains::ArchiveCommandOptions options;
    options.objects = {
        std::filesystem::path("build/main.o"),
    };
    options.output = "build/libapp.a";

    auto result = cgride::toolchains::make_archive_command(toolchain, options);

    assert(!result);
    assert(result.error().code() == cgride::core::ErrorCode::NotFound);
    assert(result.error().message() == "Archiver executable was not found.");
  }

  return 0;
}
