/**
 *
 *  @file compile_command.hpp
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
#ifndef CGRIDE_TOOLCHAINS_COMPILE_COMMAND_HPP
#define CGRIDE_TOOLCHAINS_COMPILE_COMMAND_HPP

#include <filesystem>
#include <string>
#include <vector>

#include <cgride/core/command.hpp>
#include <cgride/core/result.hpp>
#include <cgride/toolchains/language.hpp>
#include <cgride/toolchains/toolchain.hpp>

namespace cgride::toolchains
{
  /**
   * @struct CompileCommandOptions
   * @brief Options used to create one native compile command.
   */
  struct CompileCommandOptions
  {
    /**
     * @brief Source language.
     */
    Language language{Language::Cxx};

    /**
     * @brief Input source file.
     */
    std::filesystem::path source{};

    /**
     * @brief Output object file.
     */
    std::filesystem::path object{};

    /**
     * @brief Include directories.
     */
    std::vector<std::filesystem::path> include_directories{};

    /**
     * @brief Preprocessor definitions.
     */
    std::vector<std::string> definitions{};

    /**
     * @brief Extra compile options.
     */
    std::vector<std::string> options{};

    /**
     * @brief Requested C++ standard.
     */
    std::string cxx_standard{"c++23"};

    /**
     * @brief Compile with debug information.
     */
    bool debug{true};

    /**
     * @brief Enable optimization.
     */
    bool optimize{false};

    /**
     * @brief Treat warnings as errors.
     */
    bool warnings_as_errors{false};
  };

  /**
   * @brief Create a compile command.
   *
   * This function only builds command data. It does not execute the compiler.
   *
   * @param toolchain Toolchain description.
   * @param options Compile command options.
   * @return Command or validation error.
   */
  [[nodiscard]] cgride::core::Result<cgride::core::Command> make_compile_command(
      const Toolchain &toolchain,
      const CompileCommandOptions &options);

} // namespace cgride::toolchains

#endif // CGRIDE_TOOLCHAINS_COMPILE_COMMAND_HPP
