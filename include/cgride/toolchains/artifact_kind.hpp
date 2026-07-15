/**
 *
 *  @file artifact_kind.hpp
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
#ifndef CGRIDE_TOOLCHAINS_ARTIFACT_KIND_HPP
#define CGRIDE_TOOLCHAINS_ARTIFACT_KIND_HPP

#include <string_view>

namespace cgride::toolchains
{
  /**
   * @enum ArtifactKind
   * @brief Native build artifact kind.
   */
  enum class ArtifactKind
  {
    Unknown,
    ObjectFile,
    StaticLibrary,
    SharedLibrary,
    Executable
  };

  /**
   * @brief Convert an artifact kind to a stable string.
   *
   * @param kind Artifact kind.
   * @return Stable string representation.
   */
  [[nodiscard]] constexpr std::string_view to_string(ArtifactKind kind) noexcept
  {
    switch (kind)
    {
    case ArtifactKind::Unknown:
      return "Unknown";

    case ArtifactKind::ObjectFile:
      return "ObjectFile";

    case ArtifactKind::StaticLibrary:
      return "StaticLibrary";

    case ArtifactKind::SharedLibrary:
      return "SharedLibrary";

    case ArtifactKind::Executable:
      return "Executable";
    }

    return "Unknown";
  }

  /**
   * @brief Return true if the artifact kind is known.
   *
   * @param kind Artifact kind.
   * @return True when the artifact kind is not Unknown.
   */
  [[nodiscard]] constexpr bool is_known(ArtifactKind kind) noexcept
  {
    return kind != ArtifactKind::Unknown;
  }

  /**
   * @brief Return true if the artifact is a library.
   *
   * @param kind Artifact kind.
   * @return True for static and shared libraries.
   */
  [[nodiscard]] constexpr bool is_library(ArtifactKind kind) noexcept
  {
    return kind == ArtifactKind::StaticLibrary ||
           kind == ArtifactKind::SharedLibrary;
  }

  /**
   * @brief Return true if the artifact can be linked.
   *
   * @param kind Artifact kind.
   * @return True for libraries and executables.
   */
  [[nodiscard]] constexpr bool is_linked_artifact(ArtifactKind kind) noexcept
  {
    return kind == ArtifactKind::StaticLibrary ||
           kind == ArtifactKind::SharedLibrary ||
           kind == ArtifactKind::Executable;
  }

  /**
   * @brief Return true if the artifact is an object file.
   *
   * @param kind Artifact kind.
   * @return True for object files.
   */
  [[nodiscard]] constexpr bool is_object_file(ArtifactKind kind) noexcept
  {
    return kind == ArtifactKind::ObjectFile;
  }

} // namespace cgride::toolchains

#endif // CGRIDE_TOOLCHAINS_ARTIFACT_KIND_HPP
