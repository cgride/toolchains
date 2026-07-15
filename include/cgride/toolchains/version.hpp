/**
 *
 *  @file version.hpp
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
#ifndef CGRIDE_TOOLCHAINS_VERSION_HPP
#define CGRIDE_TOOLCHAINS_VERSION_HPP

#include <string_view>

namespace cgride::toolchains
{
  /**
   * @brief Major version of the Cgride toolchains module.
   */
  inline constexpr int version_major = 0;

  /**
   * @brief Minor version of the Cgride toolchains module.
   */
  inline constexpr int version_minor = 1;

  /**
   * @brief Patch version of the Cgride toolchains module.
   */
  inline constexpr int version_patch = 0;

  /**
   * @brief Public API version of the Cgride toolchains module.
   */
  inline constexpr int api_version = 1;

  /**
   * @brief Human-readable version string.
   */
  inline constexpr std::string_view version_string = "0.1.0";

} // namespace cgride::toolchains

#endif // CGRIDE_TOOLCHAINS_VERSION_HPP
