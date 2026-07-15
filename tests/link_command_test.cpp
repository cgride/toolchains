/**
 *
 *  @file link_command_test.cpp
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

#include <cgride/core/error.hpp>
#include <cgride/toolchains/link_command.hpp>

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
        .linker("/usr/bin/g++")
        .default_library_directory("/usr/lib")
        .builtin_link_option("-pthread");

    return toolchain;
  }

  [[nodiscard]] cgride::toolchains::Toolchain make_msvc_toolchain()
  {
    cgride::toolchains::Toolchain toolchain(
        cgride::toolchains::CompilerKind::Msvc,
        "MSVC");

    toolchain
        .cxx_compiler("cl")
        .linker("link")
        .default_library_directory("C:/libs")
        .builtin_link_option("kernel32.lib");

    return toolchain;
  }

} // namespace

int main()
{
  {
    auto toolchain = make_gnu_toolchain();

    cgride::toolchains::LinkCommandOptions options;
    options.artifact_kind = cgride::toolchains::ArtifactKind::Executable;
    options.objects = {
        std::filesystem::path("build/main.o"),
        std::filesystem::path("build/app.o"),
    };
    options.output = "build/app";
    options.library_directories = {
        std::filesystem::path("third_party/lib"),
    };
    options.libraries = {
        "m",
        "pthread",
    };
    options.options = {
        "-Wl,--as-needed",
    };

    auto result = cgride::toolchains::make_link_command(toolchain, options);

    assert(result);

    auto command = result.value();

    assert(command.program() == std::filesystem::path("/usr/bin/g++"));
    assert(command.search_in_path());

    const auto &args = command.args();

    assert(contains_arg(args, "-o"));
    assert(contains_arg(args, "build/app"));
    assert(contains_arg(args, "build/main.o"));
    assert(contains_arg(args, "build/app.o"));
    assert(contains_arg(args, "-L/usr/lib"));
    assert(contains_arg(args, "-Lthird_party/lib"));
    assert(contains_arg(args, "-pthread"));
    assert(contains_arg(args, "-Wl,--as-needed"));
    assert(contains_arg(args, "-lm"));
    assert(contains_arg(args, "-lpthread"));
  }

  {
    auto toolchain = make_gnu_toolchain();

    toolchain
        .target_triple("x86_64-linux-gnu")
        .sysroot("/opt/sysroot");

    cgride::toolchains::LinkCommandOptions options;
    options.artifact_kind = cgride::toolchains::ArtifactKind::SharedLibrary;
    options.objects = {
        std::filesystem::path("build/core.o"),
    };
    options.output = "build/libcore.so";
    options.shared = true;
    options.strip = true;

    auto result = cgride::toolchains::make_link_command(toolchain, options);

    assert(result);

    auto command = result.value();
    const auto &args = command.args();

    assert(contains_arg(args, "-shared"));
    assert(contains_arg(args, "-o"));
    assert(contains_arg(args, "build/libcore.so"));
    assert(contains_arg(args, "--target=x86_64-linux-gnu"));
    assert(contains_arg(args, "--sysroot=/opt/sysroot"));
    assert(contains_arg(args, "-s"));
  }

  {
    auto toolchain = make_gnu_toolchain();

    cgride::toolchains::LinkCommandOptions options;
    options.objects = {
        std::filesystem::path("build/main.o"),
    };
    options.output = "build/app";
    options.libraries = {
        "-ldl",
        "third_party/libcustom.a",
        "/usr/lib/libz.so",
        "m",
    };

    auto result = cgride::toolchains::make_link_command(toolchain, options);

    assert(result);

    auto command = result.value();
    const auto &args = command.args();

    assert(contains_arg(args, "-ldl"));
    assert(contains_arg(args, "third_party/libcustom.a"));
    assert(contains_arg(args, "/usr/lib/libz.so"));
    assert(contains_arg(args, "-lm"));
  }

  {
    auto toolchain = make_msvc_toolchain();

    cgride::toolchains::LinkCommandOptions options;
    options.artifact_kind = cgride::toolchains::ArtifactKind::Executable;
    options.objects = {
        std::filesystem::path("build/main.obj"),
        std::filesystem::path("build/app.obj"),
    };
    options.output = "build/app.exe";
    options.library_directories = {
        std::filesystem::path("third_party/lib"),
    };
    options.libraries = {
        "user32.lib",
    };
    options.options = {
        "/DEBUG",
    };

    auto result = cgride::toolchains::make_link_command(toolchain, options);

    assert(result);

    auto command = result.value();

    assert(command.program() == std::filesystem::path("link"));
    assert(command.search_in_path());

    const auto &args = command.args();

    assert(contains_arg(args, "/nologo"));
    assert(contains_arg(args, "/OUT:build/app.exe"));
    assert(contains_arg(args, "build/main.obj"));
    assert(contains_arg(args, "build/app.obj"));
    assert(contains_arg(args, "/LIBPATH:C:/libs"));
    assert(contains_arg(args, "/LIBPATH:third_party/lib"));
    assert(contains_arg(args, "kernel32.lib"));
    assert(contains_arg(args, "/DEBUG"));
    assert(contains_arg(args, "user32.lib"));
  }

  {
    auto toolchain = make_msvc_toolchain();

    cgride::toolchains::LinkCommandOptions options;
    options.artifact_kind = cgride::toolchains::ArtifactKind::SharedLibrary;
    options.objects = {
        std::filesystem::path("build/core.obj"),
    };
    options.output = "build/core.dll";
    options.shared = true;

    auto result = cgride::toolchains::make_link_command(toolchain, options);

    assert(result);

    auto command = result.value();
    const auto &args = command.args();

    assert(contains_arg(args, "/nologo"));
    assert(contains_arg(args, "/OUT:build/core.dll"));
    assert(contains_arg(args, "/DLL"));
    assert(contains_arg(args, "build/core.obj"));
  }

  {
    cgride::toolchains::Toolchain toolchain(
        cgride::toolchains::CompilerKind::Gcc,
        "Invalid GCC");

    cgride::toolchains::LinkCommandOptions options;
    options.objects = {
        std::filesystem::path("build/main.o"),
    };
    options.output = "build/app";

    auto result = cgride::toolchains::make_link_command(toolchain, options);

    assert(!result);
    assert(result.error().code() == cgride::core::ErrorCode::InvalidArgument);
    assert(result.error().message() == "Cannot create link command from an invalid toolchain.");
  }

  {
    auto toolchain = make_gnu_toolchain();

    cgride::toolchains::LinkCommandOptions options;
    options.output = "build/app";

    auto result = cgride::toolchains::make_link_command(toolchain, options);

    assert(!result);
    assert(result.error().code() == cgride::core::ErrorCode::InvalidArgument);
    assert(result.error().message() == "Cannot create link command from invalid link options.");
  }

  {
    auto toolchain = make_gnu_toolchain();

    cgride::toolchains::LinkCommandOptions options;
    options.objects = {
        std::filesystem::path("build/main.o"),
    };
    options.output = {};

    auto result = cgride::toolchains::make_link_command(toolchain, options);

    assert(!result);
    assert(result.error().code() == cgride::core::ErrorCode::InvalidArgument);
    assert(result.error().message() == "Cannot create link command from invalid link options.");
  }

  {
    auto toolchain = make_gnu_toolchain();

    cgride::toolchains::LinkCommandOptions options;
    options.artifact_kind = cgride::toolchains::ArtifactKind::StaticLibrary;
    options.objects = {
        std::filesystem::path("build/main.o"),
    };
    options.output = "build/libapp.a";

    auto result = cgride::toolchains::make_link_command(toolchain, options);

    assert(!result);
    assert(result.error().code() == cgride::core::ErrorCode::InvalidArgument);
    assert(result.error().message() == "Cannot create link command from invalid link options.");
  }

  {
    auto toolchain = make_gnu_toolchain();

    cgride::toolchains::LinkCommandOptions options;
    options.objects = {
        std::filesystem::path{},
    };
    options.output = "build/app";

    auto result = cgride::toolchains::make_link_command(toolchain, options);

    assert(!result);
    assert(result.error().code() == cgride::core::ErrorCode::InvalidArgument);
    assert(result.error().message() == "Cannot create link command from invalid link options.");
  }

  {
    cgride::toolchains::Toolchain toolchain(
        cgride::toolchains::CompilerKind::Gcc,
        "GCC");

    toolchain.cxx_compiler("/usr/bin/g++");

    cgride::toolchains::LinkCommandOptions options;
    options.objects = {
        std::filesystem::path("build/main.o"),
    };
    options.output = "build/app";

    auto result = cgride::toolchains::make_link_command(toolchain, options);

    assert(result);

    auto command = result.value();

    assert(command.program() == std::filesystem::path("/usr/bin/g++"));
  }

  return 0;
}
