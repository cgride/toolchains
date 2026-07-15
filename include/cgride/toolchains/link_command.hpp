/**
 *
 *  @file link_command.hpp
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
#ifndef CGRIDE_TOOLCHAINS_LINK_COMMAND_HPP
#define CGRIDE_TOOLCHAINS_LINK_COMMAND_HPP

#include <filesystem>
#include <string>
#include <vector>

#include <cgride/core/command.hpp>
#include <cgride/core/result.hpp>
#include <cgride/toolchains/artifact_kind.hpp>
#include <cgride/toolchains/toolchain.hpp>

namespace cgride::toolchains
{
  /**
   * @struct LinkCommandOptions
   * @brief Options used to create one native link command.
   */
  struct LinkCommandOptions
  {
    /**
     * @brief Output artifact kind.
     */
    ArtifactKind artifact_kind{ArtifactKind::Executable};

    /**
     * @brief Object files to link.
     */
    std::vector<std::filesystem::path> objects{};

    /**
     * @brief Output artifact path.
     */
    std::filesystem::path output{};

    /**
     * @brief Library search directories.
     */
    std::vector<std::filesystem::path> library_directories{};

    /**
     * @brief Libraries to link.
     */
    std::vector<std::string> libraries{};

    /**
     * @brief Extra linker options.
     */
    std::vector<std::string> options{};

    /**
     * @brief Build a position independent shared library when supported.
     */
    bool shared{false};

    /**
     * @brief Strip symbols when supported.
     */
    bool strip{false};
  };

  /**
   * @brief Create a native link command.
   *
   * This function only builds command data. It does not execute the linker.
   *
   * @param toolchain Toolchain description.
   * @param options Link command options.
   * @return Command or validation error.
   */
  [[nodiscard]] cgride::core::Result<cgride::core::Command> make_link_command(
      const Toolchain &toolchain,
      const LinkCommandOptions &options);

} // namespace cgride::toolchains

#endif // CGRIDE_TOOLCHAINS_LINK_COMMAND_HPP
