/**
 *
 *  @file discovery_test.cpp
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
#include <fstream>
#include <string>
#if defined(_WIN32)
#include <process.h>
#define get_process_id _getpid
#else
#include <unistd.h>
#define get_process_id getpid
#endif

#include <cgride/toolchains/discovery.hpp>

namespace
{
  [[nodiscard]] std::filesystem::path make_test_directory()
  {
    auto directory = std::filesystem::temp_directory_path() /
                     ("cgride_toolchains_discovery_test_" + std::to_string(::getpid()));

    std::filesystem::remove_all(directory);
    std::filesystem::create_directories(directory);

    return directory;
  }

  void write_file(const std::filesystem::path &path)
  {
    std::ofstream file(path);
    file << "fake executable\n";
  }

} // namespace

int main()
{
  {
    auto options = cgride::toolchains::default_discovery_options();

    assert(options.include_environment_path);
    assert(!options.c_compiler_names.empty());
    assert(!options.cxx_compiler_names.empty());
    assert(!options.archiver_names.empty());
    assert(!options.linker_names.empty());
  }

  {
    auto options = cgride::toolchains::default_discovery_options_for(
        cgride::toolchains::CompilerKind::Gcc);

    assert(options.c_compiler_names[0] == "gcc");
    assert(options.cxx_compiler_names[0] == "g++");
    assert(!options.archiver_names.empty());
    assert(!options.linker_names.empty());
  }

  {
    auto options = cgride::toolchains::default_discovery_options_for(
        cgride::toolchains::CompilerKind::Clang);

    assert(options.c_compiler_names[0] == "clang");
    assert(options.cxx_compiler_names[0] == "clang++");
  }

  {
    auto options = cgride::toolchains::default_discovery_options_for(
        cgride::toolchains::CompilerKind::Msvc);

    assert(options.c_compiler_names[0] == "cl");
    assert(options.cxx_compiler_names[0] == "cl");
    assert(options.archiver_names[0] == "lib");
    assert(options.linker_names[0] == "link");
  }

  {
    auto options = cgride::toolchains::default_discovery_options_for(
        cgride::toolchains::CompilerKind::Unknown);

    assert(!options.c_compiler_names.empty());
    assert(!options.cxx_compiler_names.empty());
  }

  {
    auto directory = make_test_directory();
    auto executable = directory / "cgride-fake-cxx";

    write_file(executable);

    cgride::toolchains::DiscoveryOptions options;
    options.search_paths = {directory};
    options.include_environment_path = false;

    auto found = cgride::toolchains::find_executable("cgride-fake-cxx", options);

    assert(found.has_value());
    assert(found.value() == executable);

    std::filesystem::remove_all(directory);
  }

  {
    auto directory = make_test_directory();
    auto executable = directory / "direct-tool";

    write_file(executable);

    cgride::toolchains::DiscoveryOptions options;
    options.include_environment_path = false;

    auto found = cgride::toolchains::find_executable(executable.string(), options);

    assert(found.has_value());
    assert(found.value() == executable);

    std::filesystem::remove_all(directory);
  }

  {
    cgride::toolchains::DiscoveryOptions options;
    options.include_environment_path = false;

    auto found = cgride::toolchains::find_executable("definitely-missing-cgride-tool", options);

    assert(!found.has_value());
  }

  {
    auto directory = make_test_directory();

    write_file(directory / "my-gcc");
    write_file(directory / "my-g++");
    write_file(directory / "my-ar");
    write_file(directory / "my-ld");

    cgride::toolchains::DiscoveryOptions options;
    options.search_paths = {directory};
    options.include_environment_path = false;
    options.c_compiler_names = {"my-gcc"};
    options.cxx_compiler_names = {"my-g++"};
    options.archiver_names = {"my-ar"};
    options.linker_names = {"my-ld"};

    auto result = cgride::toolchains::discover_toolchain(
        cgride::toolchains::CompilerKind::Gcc,
        options);

    assert(result);

    auto toolchain = result.value();

    assert(toolchain.compiler_kind() == cgride::toolchains::CompilerKind::Gcc);
    assert(toolchain.name() == "GCC");
    assert(toolchain.valid());

    assert(toolchain.c_compiler().value() == directory / "my-gcc");
    assert(toolchain.cxx_compiler().value() == directory / "my-g++");
    assert(toolchain.archiver().value() == directory / "my-ar");
    assert(toolchain.linker().value() == directory / "my-ld");

    std::filesystem::remove_all(directory);
  }

  {
    cgride::toolchains::DiscoveryOptions options;
    options.include_environment_path = false;
    options.cxx_compiler_names = {"missing-cxx"};

    auto result = cgride::toolchains::discover_toolchain(
        cgride::toolchains::CompilerKind::Gcc,
        options);

    assert(!result);
    assert(result.error().code() == cgride::core::ErrorCode::NotFound);
    assert(result.error().message() == "C++ compiler executable was not found.");
  }

  {
    auto directory = make_test_directory();

    write_file(directory / "auto-g++");

    cgride::toolchains::DiscoveryOptions options;
    options.search_paths = {directory};
    options.include_environment_path = false;
    options.cxx_compiler_names = {"auto-g++"};
    options.c_compiler_names = {"missing-gcc"};
    options.archiver_names = {"missing-ar"};
    options.linker_names = {"missing-ld"};

    auto result = cgride::toolchains::discover_toolchain(
        cgride::toolchains::CompilerKind::Unknown,
        options);

    assert(result);
    assert(result.value().valid());
    assert(result.value().cxx_compiler().value() == directory / "auto-g++");

    std::filesystem::remove_all(directory);
  }

  {
    cgride::toolchains::DiscoveryOptions options;
    options.include_environment_path = false;

    auto result = cgride::toolchains::discover_toolchain(
        cgride::toolchains::CompilerKind::Unknown,
        options);

    assert(!result);
    assert(result.error().code() == cgride::core::ErrorCode::NotFound);
    assert(result.error().message() == "No C++ compiler executable was found.");
  }

  {
    auto directory = make_test_directory();

    write_file(directory / "my-g++");

    cgride::toolchains::DiscoveryOptions options;
    options.search_paths = {directory};
    options.include_environment_path = false;
    options.cxx_compiler_names = {"my-g++"};
    options.c_compiler_names = {"missing-gcc"};
    options.archiver_names = {"missing-ar"};
    options.linker_names = {"missing-ld"};

    auto result = cgride::toolchains::discover_toolchain(
        cgride::toolchains::CompilerKind::Gcc,
        options);

    assert(result);

    auto toolchain = result.value();

    assert(toolchain.has_cxx_compiler());
    assert(!toolchain.has_c_compiler());
    assert(!toolchain.has_archiver());
    assert(!toolchain.has_linker());
    assert(toolchain.valid());

    std::filesystem::remove_all(directory);
  }

  return 0;
}
