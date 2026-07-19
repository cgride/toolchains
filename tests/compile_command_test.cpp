/**
 *
 *  @file compile_command_test.cpp
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

#include <cgride/toolchains/compile_command.hpp>

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
        .c_compiler("/usr/bin/gcc")
        .cxx_compiler("/usr/bin/g++")
        .archiver("/usr/bin/ar")
        .linker("/usr/bin/g++")
        .default_include_directory("/usr/include")
        .builtin_compile_option("-Wall");

    return toolchain;
  }

  [[nodiscard]] cgride::toolchains::Toolchain make_msvc_toolchain()
  {
    cgride::toolchains::Toolchain toolchain(
        cgride::toolchains::CompilerKind::Msvc,
        "MSVC");

    toolchain
        .c_compiler("cl")
        .cxx_compiler("cl")
        .archiver("lib")
        .linker("link")
        .default_include_directory("C:/include")
        .builtin_compile_option("/W4");

    return toolchain;
  }

} // namespace

int main()
{
  {
    auto toolchain = make_gnu_toolchain();

    cgride::toolchains::CompileCommandOptions options;
    options.language = cgride::toolchains::Language::Cxx;
    options.source = "src/main.cpp";
    options.object = "build/main.o";
    options.include_directories = {
        std::filesystem::path("include"),
        std::filesystem::path("src"),
    };
    options.definitions = {
        "CGRIDE_DEBUG=1",
        "APP_NAME=demo",
    };
    options.options = {
        "-fPIC",
    };
    options.cxx_standard = "c++23";
    options.debug = true;
    options.optimize = false;
    options.warnings_as_errors = true;

    auto result = cgride::toolchains::make_compile_command(toolchain, options);

    assert(result);

    auto command = result.value();

    assert(command.program() == std::filesystem::path("/usr/bin/g++"));
    assert(command.search_in_path());

    const auto &args = command.args();

    assert(contains_arg(args, "-c"));
    assert(contains_arg(args, "src/main.cpp"));
    assert(contains_arg(args, "-o"));
    assert(contains_arg(args, "build/main.o"));
    assert(contains_arg(args, "-std=c++2b"));
    assert(contains_arg(args, "-g"));
    assert(contains_arg(args, "-O0"));
    assert(contains_arg(args, "-Werror"));
    assert(contains_arg(args, "-I/usr/include"));
    assert(contains_arg(args, "-Iinclude"));
    assert(contains_arg(args, "-Isrc"));
    assert(contains_arg(args, "-DCGRIDE_DEBUG=1"));
    assert(contains_arg(args, "-DAPP_NAME=demo"));
    assert(contains_arg(args, "-Wall"));
    assert(contains_arg(args, "-fPIC"));
  }

  {
    auto toolchain = make_gnu_toolchain();

    toolchain
        .target_triple("x86_64-linux-gnu")
        .sysroot("/opt/sysroot");

    cgride::toolchains::CompileCommandOptions options;
    options.source = "src/lib.cpp";
    options.object = "build/lib.o";
    options.debug = false;
    options.optimize = true;

    auto result = cgride::toolchains::make_compile_command(toolchain, options);

    assert(result);

    auto command = result.value();
    const auto &args = command.args();

    assert(contains_arg(args, "-O2"));
    assert(contains_arg(args, "--target=x86_64-linux-gnu"));
    assert(contains_arg(args, "--sysroot=/opt/sysroot"));
  }

  {
    auto toolchain = make_msvc_toolchain();

    cgride::toolchains::CompileCommandOptions options;
    options.language = cgride::toolchains::Language::Cxx;
    options.source = "src/main.cpp";
    options.object = "build/main.obj";
    options.include_directories = {
        std::filesystem::path("include"),
    };
    options.definitions = {
        "CGRIDE_DEBUG=1",
    };
    options.options = {
        "/EHsc",
    };
    options.cxx_standard = "c++20";
    options.debug = true;
    options.optimize = true;
    options.warnings_as_errors = true;

    auto result = cgride::toolchains::make_compile_command(toolchain, options);

    assert(result);

    auto command = result.value();

    assert(command.program() == std::filesystem::path("cl"));
    assert(command.search_in_path());

    const auto &args = command.args();

    assert(contains_arg(args, "/nologo"));
    assert(contains_arg(args, "/c"));
    assert(contains_arg(args, "src/main.cpp"));
    assert(contains_arg(args, "/Fobuild/main.obj"));
    assert(contains_arg(args, "/std:c++20"));
    assert(contains_arg(args, "/Zi"));
    assert(contains_arg(args, "/O2"));
    assert(contains_arg(args, "/WX"));
    assert(contains_arg(args, "/IC:/include"));
    assert(contains_arg(args, "/Iinclude"));
    assert(contains_arg(args, "/DCGRIDE_DEBUG=1"));
    assert(contains_arg(args, "/W4"));
    assert(contains_arg(args, "/EHsc"));
  }

  {
    auto toolchain = make_gnu_toolchain();

    cgride::toolchains::CompileCommandOptions options;
    options.language = cgride::toolchains::Language::C;
    options.source = "src/main.c";
    options.object = "build/main.o";

    auto result = cgride::toolchains::make_compile_command(toolchain, options);

    assert(result);

    auto command = result.value();

    assert(command.program() == std::filesystem::path("/usr/bin/gcc"));

    const auto &args = command.args();

    assert(contains_arg(args, "-c"));
    assert(contains_arg(args, "src/main.c"));
    assert(!contains_arg(args, "-std=c++23"));
  }

  {
    cgride::toolchains::Toolchain toolchain(
        cgride::toolchains::CompilerKind::Gcc,
        "Invalid GCC");

    cgride::toolchains::CompileCommandOptions options;
    options.source = "src/main.cpp";
    options.object = "build/main.o";

    auto result = cgride::toolchains::make_compile_command(toolchain, options);

    assert(!result);
    assert(result.error().code() == cgride::core::ErrorCode::InvalidArgument);
    assert(result.error().message() == "Cannot create compile command from an invalid toolchain.");
  }

  {
    auto toolchain = make_gnu_toolchain();

    cgride::toolchains::CompileCommandOptions options;
    options.source = std::filesystem::path{};
    options.object = "build/main.o";

    auto result = cgride::toolchains::make_compile_command(toolchain, options);

    assert(!result);
    assert(result.error().code() == cgride::core::ErrorCode::InvalidArgument);
    assert(result.error().message() == "Cannot create compile command from invalid compile options.");
  }

  {
    auto toolchain = make_gnu_toolchain();

    cgride::toolchains::CompileCommandOptions options;
    options.source = "src/main.cpp";
    options.object = std::filesystem::path{};

    auto result = cgride::toolchains::make_compile_command(toolchain, options);

    assert(!result);
    assert(result.error().code() == cgride::core::ErrorCode::InvalidArgument);
    assert(result.error().message() == "Cannot create compile command from invalid compile options.");
  }

  {
    auto toolchain = make_gnu_toolchain();

    cgride::toolchains::CompileCommandOptions options;
    options.language = cgride::toolchains::Language::Unknown;
    options.source = "src/main.cpp";
    options.object = "build/main.o";

    auto result = cgride::toolchains::make_compile_command(toolchain, options);

    assert(!result);
    assert(result.error().code() == cgride::core::ErrorCode::InvalidArgument);
    assert(result.error().message() == "Cannot create compile command from invalid compile options.");
  }

  {
    cgride::toolchains::Toolchain toolchain(
        cgride::toolchains::CompilerKind::Gcc,
        "GCC");

    toolchain.cxx_compiler("/usr/bin/g++");

    cgride::toolchains::CompileCommandOptions options;
    options.language = cgride::toolchains::Language::C;
    options.source = "src/main.c";
    options.object = "build/main.o";

    auto result = cgride::toolchains::make_compile_command(toolchain, options);

    assert(!result);
    assert(result.error().code() == cgride::core::ErrorCode::NotFound);
    assert(result.error().message() == "Compiler executable was not found for requested language.");
  }

  return 0;
}
