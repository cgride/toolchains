/**
 *
 *  @file toolchain.hpp
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
#ifndef CGRIDE_TOOLCHAINS_TOOLCHAIN_HPP
#define CGRIDE_TOOLCHAINS_TOOLCHAIN_HPP

#include <filesystem>
#include <optional>
#include <string>
#include <vector>

#include <cgride/toolchains/compiler_kind.hpp>
#include <cgride/toolchains/language.hpp>

namespace cgride::toolchains
{
  /**
   * @class Toolchain
   * @brief Native compiler toolchain description.
   *
   * Toolchain stores discovered compiler paths and default options. It does
   * not execute commands, inspect compiler versions or build targets.
   */
  class Toolchain
  {
  public:
    /**
     * @brief Construct an empty toolchain.
     */
    Toolchain() = default;

    /**
     * @brief Construct a toolchain.
     *
     * @param kind Compiler family.
     */
    explicit Toolchain(CompilerKind kind);

    /**
     * @brief Construct a named toolchain.
     *
     * @param kind Compiler family.
     * @param name Human-readable toolchain name.
     */
    Toolchain(CompilerKind kind, std::string name);

    /**
     * @brief Set the compiler family.
     *
     * @param kind Compiler family.
     * @return Reference to this toolchain.
     */
    Toolchain &compiler_kind(CompilerKind kind) noexcept;

    /**
     * @brief Set the human-readable toolchain name.
     *
     * @param name Toolchain name.
     * @return Reference to this toolchain.
     */
    Toolchain &name(std::string name);

    /**
     * @brief Set the C compiler path.
     *
     * @param path Compiler path.
     * @return Reference to this toolchain.
     */
    Toolchain &c_compiler(std::filesystem::path path);

    /**
     * @brief Set the C++ compiler path.
     *
     * @param path Compiler path.
     * @return Reference to this toolchain.
     */
    Toolchain &cxx_compiler(std::filesystem::path path);

    /**
     * @brief Set the archiver path.
     *
     * @param path Archiver path.
     * @return Reference to this toolchain.
     */
    Toolchain &archiver(std::filesystem::path path);

    /**
     * @brief Set the linker path.
     *
     * @param path Linker path.
     * @return Reference to this toolchain.
     */
    Toolchain &linker(std::filesystem::path path);

    /**
     * @brief Set the target triple.
     *
     * @param triple Target triple.
     * @return Reference to this toolchain.
     */
    Toolchain &target_triple(std::string triple);

    /**
     * @brief Set the sysroot path.
     *
     * @param path Sysroot path.
     * @return Reference to this toolchain.
     */
    Toolchain &sysroot(std::filesystem::path path);

    /**
     * @brief Add a default include directory.
     *
     * @param path Include directory path.
     * @return Reference to this toolchain.
     */
    Toolchain &default_include_directory(std::filesystem::path path);

    /**
     * @brief Add a default library directory.
     *
     * @param path Library directory path.
     * @return Reference to this toolchain.
     */
    Toolchain &default_library_directory(std::filesystem::path path);

    /**
     * @brief Add a builtin compile option.
     *
     * @param option Compile option.
     * @return Reference to this toolchain.
     */
    Toolchain &builtin_compile_option(std::string option);

    /**
     * @brief Add a builtin link option.
     *
     * @param option Link option.
     * @return Reference to this toolchain.
     */
    Toolchain &builtin_link_option(std::string option);

    /**
     * @brief Access the compiler family.
     */
    [[nodiscard]] CompilerKind compiler_kind() const noexcept;

    /**
     * @brief Access the human-readable toolchain name.
     */
    [[nodiscard]] const std::string &name() const noexcept;

    /**
     * @brief Access the C compiler path.
     */
    [[nodiscard]] const std::optional<std::filesystem::path> &c_compiler() const noexcept;

    /**
     * @brief Access the C++ compiler path.
     */
    [[nodiscard]] const std::optional<std::filesystem::path> &cxx_compiler() const noexcept;

    /**
     * @brief Access the archiver path.
     */
    [[nodiscard]] const std::optional<std::filesystem::path> &archiver() const noexcept;

    /**
     * @brief Access the linker path.
     */
    [[nodiscard]] const std::optional<std::filesystem::path> &linker() const noexcept;

    /**
     * @brief Access the compiler path for a language.
     *
     * @param language Source language.
     * @return Compiler path when available.
     */
    [[nodiscard]] std::optional<std::filesystem::path> compiler_for(Language language) const;

    /**
     * @brief Access the target triple.
     */
    [[nodiscard]] const std::string &target_triple() const noexcept;

    /**
     * @brief Access the sysroot path.
     */
    [[nodiscard]] const std::optional<std::filesystem::path> &sysroot() const noexcept;

    /**
     * @brief Access default include directories.
     */
    [[nodiscard]] const std::vector<std::filesystem::path> &default_include_directories() const noexcept;

    /**
     * @brief Access default library directories.
     */
    [[nodiscard]] const std::vector<std::filesystem::path> &default_library_directories() const noexcept;

    /**
     * @brief Access builtin compile options.
     */
    [[nodiscard]] const std::vector<std::string> &builtin_compile_options() const noexcept;

    /**
     * @brief Access builtin link options.
     */
    [[nodiscard]] const std::vector<std::string> &builtin_link_options() const noexcept;

    /**
     * @brief Return true if a C compiler path is available.
     */
    [[nodiscard]] bool has_c_compiler() const noexcept;

    /**
     * @brief Return true if a C++ compiler path is available.
     */
    [[nodiscard]] bool has_cxx_compiler() const noexcept;

    /**
     * @brief Return true if an archiver path is available.
     */
    [[nodiscard]] bool has_archiver() const noexcept;

    /**
     * @brief Return true if a linker path is available.
     */
    [[nodiscard]] bool has_linker() const noexcept;

    /**
     * @brief Return true if the toolchain can compile C++.
     */
    [[nodiscard]] bool can_compile_cxx() const noexcept;

    /**
     * @brief Return true when the toolchain has a known compiler family and C++ compiler.
     */
    [[nodiscard]] bool valid() const noexcept;

  private:
    CompilerKind compiler_kind_{CompilerKind::Unknown};
    std::string name_{};
    std::optional<std::filesystem::path> c_compiler_{};
    std::optional<std::filesystem::path> cxx_compiler_{};
    std::optional<std::filesystem::path> archiver_{};
    std::optional<std::filesystem::path> linker_{};
    std::string target_triple_{};
    std::optional<std::filesystem::path> sysroot_{};
    std::vector<std::filesystem::path> default_include_directories_{};
    std::vector<std::filesystem::path> default_library_directories_{};
    std::vector<std::string> builtin_compile_options_{};
    std::vector<std::string> builtin_link_options_{};
  };

} // namespace cgride::toolchains

#endif // CGRIDE_TOOLCHAINS_TOOLCHAIN_HPP
