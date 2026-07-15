/**
 *
 *  @file archive_command.hpp
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
#ifndef CGRIDE_TOOLCHAINS_ARCHIVE_COMMAND_HPP
#define CGRIDE_TOOLCHAINS_ARCHIVE_COMMAND_HPP

#include <filesystem>
#include <vector>

#include <cgride/core/command.hpp>
#include <cgride/core/result.hpp>
#include <cgride/toolchains/toolchain.hpp>

namespace cgride::toolchains
{
  /**
   * @struct ArchiveCommandOptions
   * @brief Options used to create one static library archive command.
   */
  struct ArchiveCommandOptions
  {
    /**
     * @brief Object files to archive.
     */
    std::vector<std::filesystem::path> objects{};

    /**
     * @brief Output static library path.
     */
    std::filesystem::path output{};

    /**
     * @brief Replace existing archive members when supported.
     */
    bool replace{true};

    /**
     * @brief Create the archive when it does not exist.
     */
    bool create{true};

    /**
     * @brief Write archive index when supported.
     */
    bool index{true};
  };

  /**
   * @brief Create a static library archive command.
   *
   * This function only builds command data. It does not execute the archiver.
   *
   * @param toolchain Toolchain description.
   * @param options Archive command options.
   * @return Command or validation error.
   */
  [[nodiscard]] cgride::core::Result<cgride::core::Command> make_archive_command(
      const Toolchain &toolchain,
      const ArchiveCommandOptions &options);

} // namespace cgride::toolchains

#endif // CGRIDE_TOOLCHAINS_ARCHIVE_COMMAND_HPP
