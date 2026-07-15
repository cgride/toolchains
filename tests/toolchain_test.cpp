/**
 *
 *  @file toolchain_test.cpp
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
#include <string_view>

#include <cgride/toolchains/artifact_kind.hpp>
#include <cgride/toolchains/compiler_kind.hpp>
#include <cgride/toolchains/language.hpp>
#include <cgride/toolchains/toolchain.hpp>

int main()
{
  {
    using cgride::toolchains::Language;
    using cgride::toolchains::to_string;

    assert(to_string(Language::Unknown) == std::string_view("Unknown"));
    assert(to_string(Language::C) == std::string_view("C"));
    assert(to_string(Language::Cxx) == std::string_view("Cxx"));

    assert(!cgride::toolchains::is_known(Language::Unknown));
    assert(cgride::toolchains::is_known(Language::C));
    assert(cgride::toolchains::is_known(Language::Cxx));

    assert(cgride::toolchains::is_c(Language::C));
    assert(!cgride::toolchains::is_c(Language::Cxx));

    assert(cgride::toolchains::is_cxx(Language::Cxx));
    assert(!cgride::toolchains::is_cxx(Language::C));
  }

  {
    using cgride::toolchains::CompilerKind;
    using cgride::toolchains::to_string;

    assert(to_string(CompilerKind::Unknown) == std::string_view("Unknown"));
    assert(to_string(CompilerKind::Gcc) == std::string_view("Gcc"));
    assert(to_string(CompilerKind::Clang) == std::string_view("Clang"));
    assert(to_string(CompilerKind::AppleClang) == std::string_view("AppleClang"));
    assert(to_string(CompilerKind::Msvc) == std::string_view("Msvc"));
    assert(to_string(CompilerKind::MinGw) == std::string_view("MinGw"));

    assert(!cgride::toolchains::is_known(CompilerKind::Unknown));
    assert(cgride::toolchains::is_known(CompilerKind::Gcc));

    assert(cgride::toolchains::is_gnu_like(CompilerKind::Gcc));
    assert(cgride::toolchains::is_gnu_like(CompilerKind::Clang));
    assert(cgride::toolchains::is_gnu_like(CompilerKind::AppleClang));
    assert(cgride::toolchains::is_gnu_like(CompilerKind::MinGw));
    assert(!cgride::toolchains::is_gnu_like(CompilerKind::Msvc));

    assert(cgride::toolchains::is_msvc_like(CompilerKind::Msvc));
    assert(!cgride::toolchains::is_msvc_like(CompilerKind::Gcc));

    assert(cgride::toolchains::is_clang_family(CompilerKind::Clang));
    assert(cgride::toolchains::is_clang_family(CompilerKind::AppleClang));
    assert(!cgride::toolchains::is_clang_family(CompilerKind::Gcc));
  }

  {
    using cgride::toolchains::ArtifactKind;
    using cgride::toolchains::to_string;

    assert(to_string(ArtifactKind::Unknown) == std::string_view("Unknown"));
    assert(to_string(ArtifactKind::ObjectFile) == std::string_view("ObjectFile"));
    assert(to_string(ArtifactKind::StaticLibrary) == std::string_view("StaticLibrary"));
    assert(to_string(ArtifactKind::SharedLibrary) == std::string_view("SharedLibrary"));
    assert(to_string(ArtifactKind::Executable) == std::string_view("Executable"));

    assert(!cgride::toolchains::is_known(ArtifactKind::Unknown));
    assert(cgride::toolchains::is_known(ArtifactKind::ObjectFile));

    assert(cgride::toolchains::is_library(ArtifactKind::StaticLibrary));
    assert(cgride::toolchains::is_library(ArtifactKind::SharedLibrary));
    assert(!cgride::toolchains::is_library(ArtifactKind::Executable));

    assert(cgride::toolchains::is_linked_artifact(ArtifactKind::StaticLibrary));
    assert(cgride::toolchains::is_linked_artifact(ArtifactKind::SharedLibrary));
    assert(cgride::toolchains::is_linked_artifact(ArtifactKind::Executable));
    assert(!cgride::toolchains::is_linked_artifact(ArtifactKind::ObjectFile));

    assert(cgride::toolchains::is_object_file(ArtifactKind::ObjectFile));
    assert(!cgride::toolchains::is_object_file(ArtifactKind::Executable));
  }

  {
    cgride::toolchains::Toolchain toolchain;

    assert(toolchain.compiler_kind() == cgride::toolchains::CompilerKind::Unknown);
    assert(toolchain.name().empty());

    assert(!toolchain.c_compiler().has_value());
    assert(!toolchain.cxx_compiler().has_value());
    assert(!toolchain.archiver().has_value());
    assert(!toolchain.linker().has_value());

    assert(toolchain.target_triple().empty());
    assert(!toolchain.sysroot().has_value());

    assert(toolchain.default_include_directories().empty());
    assert(toolchain.default_library_directories().empty());
    assert(toolchain.builtin_compile_options().empty());
    assert(toolchain.builtin_link_options().empty());

    assert(!toolchain.has_c_compiler());
    assert(!toolchain.has_cxx_compiler());
    assert(!toolchain.has_archiver());
    assert(!toolchain.has_linker());
    assert(!toolchain.can_compile_cxx());
    assert(!toolchain.valid());
  }

  {
    cgride::toolchains::Toolchain toolchain(
        cgride::toolchains::CompilerKind::Gcc);

    assert(toolchain.compiler_kind() == cgride::toolchains::CompilerKind::Gcc);
    assert(!toolchain.valid());
  }

  {
    cgride::toolchains::Toolchain toolchain(
        cgride::toolchains::CompilerKind::Clang,
        "Clang");

    assert(toolchain.compiler_kind() == cgride::toolchains::CompilerKind::Clang);
    assert(toolchain.name() == "Clang");

    toolchain
        .name("Local Clang")
        .compiler_kind(cgride::toolchains::CompilerKind::Gcc)
        .c_compiler("/usr/bin/gcc")
        .cxx_compiler("/usr/bin/g++")
        .archiver("/usr/bin/ar")
        .linker("/usr/bin/g++")
        .target_triple("x86_64-linux-gnu")
        .sysroot("/opt/sysroot")
        .default_include_directory("/usr/include")
        .default_include_directory("/usr/local/include")
        .default_library_directory("/usr/lib")
        .builtin_compile_option("-Wall")
        .builtin_compile_option("-Wextra")
        .builtin_link_option("-pthread");

    assert(toolchain.name() == "Local Clang");
    assert(toolchain.compiler_kind() == cgride::toolchains::CompilerKind::Gcc);

    assert(toolchain.has_c_compiler());
    assert(toolchain.has_cxx_compiler());
    assert(toolchain.has_archiver());
    assert(toolchain.has_linker());
    assert(toolchain.can_compile_cxx());
    assert(toolchain.valid());

    assert(toolchain.c_compiler().value() == std::filesystem::path("/usr/bin/gcc"));
    assert(toolchain.cxx_compiler().value() == std::filesystem::path("/usr/bin/g++"));
    assert(toolchain.archiver().value() == std::filesystem::path("/usr/bin/ar"));
    assert(toolchain.linker().value() == std::filesystem::path("/usr/bin/g++"));

    assert(toolchain.compiler_for(cgride::toolchains::Language::C).value() ==
           std::filesystem::path("/usr/bin/gcc"));

    assert(toolchain.compiler_for(cgride::toolchains::Language::Cxx).value() ==
           std::filesystem::path("/usr/bin/g++"));

    assert(!toolchain.compiler_for(cgride::toolchains::Language::Unknown).has_value());

    assert(toolchain.target_triple() == "x86_64-linux-gnu");
    assert(toolchain.sysroot().value() == std::filesystem::path("/opt/sysroot"));

    assert(toolchain.default_include_directories().size() == 2);
    assert(toolchain.default_include_directories()[0] == std::filesystem::path("/usr/include"));
    assert(toolchain.default_include_directories()[1] == std::filesystem::path("/usr/local/include"));

    assert(toolchain.default_library_directories().size() == 1);
    assert(toolchain.default_library_directories()[0] == std::filesystem::path("/usr/lib"));

    assert(toolchain.builtin_compile_options().size() == 2);
    assert(toolchain.builtin_compile_options()[0] == "-Wall");
    assert(toolchain.builtin_compile_options()[1] == "-Wextra");

    assert(toolchain.builtin_link_options().size() == 1);
    assert(toolchain.builtin_link_options()[0] == "-pthread");
  }

  {
    cgride::toolchains::Toolchain toolchain(
        cgride::toolchains::CompilerKind::Gcc);

    toolchain.cxx_compiler({});

    assert(!toolchain.has_cxx_compiler());
    assert(!toolchain.can_compile_cxx());
    assert(!toolchain.valid());
  }

  return 0;
}
