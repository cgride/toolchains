/**
 *
 *  @file language.hpp
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
#ifndef CGRIDE_TOOLCHAINS_LANGUAGE_HPP
#define CGRIDE_TOOLCHAINS_LANGUAGE_HPP

#include <string_view>

namespace cgride::toolchains
{
  /**
   * @enum Language
   * @brief Source language handled by a compiler toolchain.
   */
  enum class Language
  {
    Unknown,
    C,
    Cxx
  };

  /**
   * @brief Convert a language to a stable string.
   *
   * @param language Source language.
   * @return Stable string representation.
   */
  [[nodiscard]] constexpr std::string_view to_string(Language language) noexcept
  {
    switch (language)
    {
    case Language::Unknown:
      return "Unknown";

    case Language::C:
      return "C";

    case Language::Cxx:
      return "Cxx";
    }

    return "Unknown";
  }

  /**
   * @brief Return true if the language is known.
   *
   * @param language Source language.
   * @return True when the language is not Unknown.
   */
  [[nodiscard]] constexpr bool is_known(Language language) noexcept
  {
    return language != Language::Unknown;
  }

  /**
   * @brief Return true if the language is C.
   *
   * @param language Source language.
   * @return True for C.
   */
  [[nodiscard]] constexpr bool is_c(Language language) noexcept
  {
    return language == Language::C;
  }

  /**
   * @brief Return true if the language is C++.
   *
   * @param language Source language.
   * @return True for C++.
   */
  [[nodiscard]] constexpr bool is_cxx(Language language) noexcept
  {
    return language == Language::Cxx;
  }

} // namespace cgride::toolchains

#endif // CGRIDE_TOOLCHAINS_LANGUAGE_HPP
