/**
 *
 *  @file discovery.hpp
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
#ifndef CGRIDE_TOOLCHAINS_DISCOVERY_HPP
#define CGRIDE_TOOLCHAINS_DISCOVERY_HPP

#include <filesystem>
#include <optional>
#include <string>
#include <vector>

#include <cgride/core/result.hpp>
#include <cgride/toolchains/compiler_kind.hpp>
#include <cgride/toolchains/toolchain.hpp>

namespace cgride::toolchains
{
  /**
   * @struct DiscoveryOptions
   * @brief Options used to discover native compiler executables.
   */
  struct DiscoveryOptions
  {
    /**
     * @brief Search directories checked before PATH entries.
     */
    std::vector<std::filesystem::path> search_paths{};

    /**
     * @brief Whether PATH from the process environment should be searched.
     */
    bool include_environment_path{true};

    /**
     * @brief Candidate C compiler executable names.
     */
    std::vector<std::string> c_compiler_names{};

    /**
     * @brief Candidate C++ compiler executable names.
     */
    std::vector<std::string> cxx_compiler_names{};

    /**
     * @brief Candidate archiver executable names.
     */
    std::vector<std::string> archiver_names{};

    /**
     * @brief Candidate linker executable names.
     */
    std::vector<std::string> linker_names{};
  };

  /**
   * @brief Return default discovery options for known native toolchains.
   *
   * @return Discovery options.
   */
  [[nodiscard]] DiscoveryOptions default_discovery_options();

  /**
   * @brief Return default executable names for a compiler kind.
   *
   * @param kind Compiler kind.
   * @return Discovery options containing compiler-specific names.
   */
  [[nodiscard]] DiscoveryOptions default_discovery_options_for(CompilerKind kind);

  /**
   * @brief Find an executable by name.
   *
   * @param name Executable name.
   * @param options Discovery options.
   * @return Executable path when found.
   */
  [[nodiscard]] std::optional<std::filesystem::path> find_executable(
      const std::string &name,
      const DiscoveryOptions &options = default_discovery_options());

  /**
   * @brief Discover one toolchain for a preferred compiler kind.
   *
   * This function checks executable presence only. It does not run the
   * compiler, parse version output or inspect the host system deeply.
   *
   * @param preferred Preferred compiler kind.
   * @param options Discovery options.
   * @return Discovered toolchain or an error.
   */
  [[nodiscard]] cgride::core::Result<Toolchain> discover_toolchain(
      CompilerKind preferred,
      const DiscoveryOptions &options);

  /**
   * @brief Discover one toolchain for a preferred compiler kind.
   *
   * @param preferred Preferred compiler kind.
   * @return Discovered toolchain or an error.
   */
  [[nodiscard]] cgride::core::Result<Toolchain> discover_toolchain(
      CompilerKind preferred);

  /**
   * @brief Discover all known toolchains found in search paths.
   *
   * @param options Base discovery options.
   * @return Discovered toolchains.
   */
  [[nodiscard]] std::vector<Toolchain> discover_known_toolchains(
      const DiscoveryOptions &options = default_discovery_options());

} // namespace cgride::toolchains

#endif // CGRIDE_TOOLCHAINS_DISCOVERY_HPP
