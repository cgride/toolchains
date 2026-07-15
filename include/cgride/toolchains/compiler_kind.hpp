/**
 *
 *  @file compiler_kind.hpp
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
#ifndef CGRIDE_TOOLCHAINS_COMPILER_KIND_HPP
#define CGRIDE_TOOLCHAINS_COMPILER_KIND_HPP

#include <string_view>

namespace cgride::toolchains
{
  /**
   * @enum CompilerKind
   * @brief Known native compiler family.
   */
  enum class CompilerKind
  {
    Unknown,
    Gcc,
    Clang,
    AppleClang,
    Msvc,
    MinGw
  };

  /**
   * @brief Convert a compiler kind to a stable string.
   *
   * @param kind Compiler kind.
   * @return Stable string representation.
   */
  [[nodiscard]] constexpr std::string_view to_string(CompilerKind kind) noexcept
  {
    switch (kind)
    {
    case CompilerKind::Unknown:
      return "Unknown";

    case CompilerKind::Gcc:
      return "Gcc";

    case CompilerKind::Clang:
      return "Clang";

    case CompilerKind::AppleClang:
      return "AppleClang";

    case CompilerKind::Msvc:
      return "Msvc";

    case CompilerKind::MinGw:
      return "MinGw";
    }

    return "Unknown";
  }

  /**
   * @brief Return true if the compiler kind is known.
   *
   * @param kind Compiler kind.
   * @return True when the compiler kind is not Unknown.
   */
  [[nodiscard]] constexpr bool is_known(CompilerKind kind) noexcept
  {
    return kind != CompilerKind::Unknown;
  }

  /**
   * @brief Return true if the compiler uses GCC-like command flags.
   *
   * @param kind Compiler kind.
   * @return True for GCC-like compilers.
   */
  [[nodiscard]] constexpr bool is_gnu_like(CompilerKind kind) noexcept
  {
    return kind == CompilerKind::Gcc ||
           kind == CompilerKind::Clang ||
           kind == CompilerKind::AppleClang ||
           kind == CompilerKind::MinGw;
  }

  /**
   * @brief Return true if the compiler uses MSVC-like command flags.
   *
   * @param kind Compiler kind.
   * @return True for MSVC-like compilers.
   */
  [[nodiscard]] constexpr bool is_msvc_like(CompilerKind kind) noexcept
  {
    return kind == CompilerKind::Msvc;
  }

  /**
   * @brief Return true if the compiler is a Clang family compiler.
   *
   * @param kind Compiler kind.
   * @return True for Clang and AppleClang.
   */
  [[nodiscard]] constexpr bool is_clang_family(CompilerKind kind) noexcept
  {
    return kind == CompilerKind::Clang ||
           kind == CompilerKind::AppleClang;
  }

} // namespace cgride::toolchains

#endif // CGRIDE_TOOLCHAINS_COMPILER_KIND_HPP
