/**
 *
 *  @file discovery.cpp
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
#include <cgride/toolchains/discovery.hpp>

#include <cstdlib>
#include <sstream>
#include <string>
#include <utility>

#include <cgride/core/error.hpp>
#include <cgride/core/platform.hpp>

namespace cgride::toolchains
{
  namespace
  {
    using cgride::core::Error;
    using cgride::core::ErrorCode;

    [[nodiscard]] bool is_windows_host() noexcept
    {
      return cgride::core::host_platform() == cgride::core::Platform::Windows;
    }

    [[nodiscard]] std::vector<std::string> executable_names_for_host(const std::string &name)
    {
      if (!is_windows_host())
      {
        return {name};
      }

      if (name.ends_with(".exe") ||
          name.ends_with(".bat") ||
          name.ends_with(".cmd"))
      {
        return {name};
      }

      return {
          name,
          name + ".exe",
          name + ".bat",
          name + ".cmd",
      };
    }

    [[nodiscard]] std::vector<std::filesystem::path> environment_path_entries()
    {
      std::vector<std::filesystem::path> entries;

      const auto *path = std::getenv("PATH");

      if (path == nullptr)
      {
        return entries;
      }

      std::stringstream stream(path);
      std::string entry;

      const auto separator = cgride::core::path_list_separator(cgride::core::host_platform());

      while (std::getline(stream, entry, separator))
      {
        if (!entry.empty())
        {
          entries.emplace_back(entry);
        }
      }

      return entries;
    }

    [[nodiscard]] std::vector<std::filesystem::path> search_paths_for(
        const DiscoveryOptions &options)
    {
      auto paths = options.search_paths;

      if (options.include_environment_path)
      {
        auto environment_paths = environment_path_entries();

        for (auto &path : environment_paths)
        {
          paths.push_back(std::move(path));
        }
      }

      return paths;
    }

    [[nodiscard]] std::optional<std::filesystem::path> find_first(
        const std::vector<std::string> &names,
        const DiscoveryOptions &options)
    {
      for (const auto &name : names)
      {
        auto found = find_executable(name, options);

        if (found.has_value())
        {
          return found;
        }
      }

      return std::nullopt;
    }

    [[nodiscard]] std::string compiler_display_name(CompilerKind kind)
    {
      switch (kind)
      {
      case CompilerKind::Gcc:
        return "GCC";

      case CompilerKind::Clang:
        return "Clang";

      case CompilerKind::AppleClang:
        return "Apple Clang";

      case CompilerKind::Msvc:
        return "MSVC";

      case CompilerKind::MinGw:
        return "MinGW";

      case CompilerKind::Unknown:
        return "Unknown";
      }

      return "Unknown";
    }

  } // namespace

  DiscoveryOptions default_discovery_options()
  {
    DiscoveryOptions options;

    options.c_compiler_names = {
        "cc",
        "gcc",
        "clang",
        "cl",
    };

    options.cxx_compiler_names = {
        "c++",
        "g++",
        "clang++",
        "cl",
    };

    options.archiver_names = {
        "ar",
        "llvm-ar",
        "lib",
    };

    options.linker_names = {
        "ld",
        "lld",
        "link",
    };

    return options;
  }

  DiscoveryOptions default_discovery_options_for(CompilerKind kind)
  {
    DiscoveryOptions options;

    switch (kind)
    {
    case CompilerKind::Gcc:
      options.c_compiler_names = {"gcc", "cc"};
      options.cxx_compiler_names = {"g++", "c++"};
      options.archiver_names = {"gcc-ar", "ar"};
      options.linker_names = {"g++", "ld"};
      break;

    case CompilerKind::Clang:
      options.c_compiler_names = {"clang", "cc"};
      options.cxx_compiler_names = {"clang++", "c++"};
      options.archiver_names = {"llvm-ar", "ar"};
      options.linker_names = {"clang++", "ld.lld", "lld", "ld"};
      break;

    case CompilerKind::AppleClang:
      options.c_compiler_names = {"clang", "cc"};
      options.cxx_compiler_names = {"clang++", "c++"};
      options.archiver_names = {"ar", "llvm-ar"};
      options.linker_names = {"clang++", "ld"};
      break;

    case CompilerKind::Msvc:
      options.c_compiler_names = {"cl"};
      options.cxx_compiler_names = {"cl"};
      options.archiver_names = {"lib"};
      options.linker_names = {"link"};
      break;

    case CompilerKind::MinGw:
      options.c_compiler_names = {"gcc", "x86_64-w64-mingw32-gcc"};
      options.cxx_compiler_names = {"g++", "x86_64-w64-mingw32-g++"};
      options.archiver_names = {"ar", "x86_64-w64-mingw32-ar"};
      options.linker_names = {"g++", "x86_64-w64-mingw32-g++"};
      break;

    case CompilerKind::Unknown:
      return default_discovery_options();
    }

    return options;
  }

  std::optional<std::filesystem::path> find_executable(
      const std::string &name,
      const DiscoveryOptions &options)
  {
    if (name.empty())
    {
      return std::nullopt;
    }

    const auto direct_path = std::filesystem::path(name);

    if (direct_path.has_parent_path() &&
        std::filesystem::exists(direct_path) &&
        std::filesystem::is_regular_file(direct_path))
    {
      return direct_path;
    }

    const auto paths = search_paths_for(options);

    for (const auto &directory : paths)
    {
      if (directory.empty())
      {
        continue;
      }

      for (const auto &candidate_name : executable_names_for_host(name))
      {
        auto candidate = directory / candidate_name;

        if (std::filesystem::exists(candidate) &&
            std::filesystem::is_regular_file(candidate))
        {
          return candidate;
        }
      }
    }

    return std::nullopt;
  }

  cgride::core::Result<Toolchain> discover_toolchain(
      CompilerKind preferred,
      const DiscoveryOptions &options)
  {
    if (!is_known(preferred))
    {
      return Error(
          ErrorCode::InvalidArgument,
          "Cannot discover an unknown compiler kind.");
    }

    auto effective = default_discovery_options_for(preferred);

    if (!options.search_paths.empty())
    {
      effective.search_paths = options.search_paths;
    }

    effective.include_environment_path = options.include_environment_path;

    if (!options.c_compiler_names.empty())
    {
      effective.c_compiler_names = options.c_compiler_names;
    }

    if (!options.cxx_compiler_names.empty())
    {
      effective.cxx_compiler_names = options.cxx_compiler_names;
    }

    if (!options.archiver_names.empty())
    {
      effective.archiver_names = options.archiver_names;
    }

    if (!options.linker_names.empty())
    {
      effective.linker_names = options.linker_names;
    }

    auto cxx_compiler = find_first(effective.cxx_compiler_names, effective);

    if (!cxx_compiler.has_value())
    {
      return Error(
          ErrorCode::NotFound,
          "C++ compiler executable was not found.",
          compiler_display_name(preferred));
    }

    Toolchain toolchain(preferred, compiler_display_name(preferred));

    toolchain.cxx_compiler(*cxx_compiler);

    auto c_compiler = find_first(effective.c_compiler_names, effective);

    if (c_compiler.has_value())
    {
      toolchain.c_compiler(*c_compiler);
    }

    auto archiver = find_first(effective.archiver_names, effective);

    if (archiver.has_value())
    {
      toolchain.archiver(*archiver);
    }

    auto linker = find_first(effective.linker_names, effective);

    if (linker.has_value())
    {
      toolchain.linker(*linker);
    }

    return toolchain;
  }

  cgride::core::Result<Toolchain> discover_toolchain(CompilerKind preferred)
  {
    return discover_toolchain(preferred, default_discovery_options_for(preferred));
  }

  std::vector<Toolchain> discover_known_toolchains(const DiscoveryOptions &options)
  {
    std::vector<Toolchain> toolchains;

    const auto known = {
        CompilerKind::Gcc,
        CompilerKind::Clang,
        CompilerKind::AppleClang,
        CompilerKind::Msvc,
        CompilerKind::MinGw,
    };

    for (const auto kind : known)
    {
      auto result = discover_toolchain(kind, options);

      if (result)
      {
        toolchains.push_back(std::move(result.value()));
      }
    }

    return toolchains;
  }

} // namespace cgride::toolchains
