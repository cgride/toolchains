cmake_minimum_required(VERSION 3.22)

project(
  cgride-toolchains
  VERSION 0.1.0
  DESCRIPTION "Native compiler discovery and command generation for Cgride."
  HOMEPAGE_URL "https://github.com/cgride/toolchains"
  LANGUAGES CXX
)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

option(CGRIDE_TOOLCHAINS_BUILD_TESTS "Build Cgride toolchains tests" ON)
option(CGRIDE_TOOLCHAINS_INSTALL "Install Cgride toolchains" ON)

if(NOT TARGET cgride::core)
  find_package(cgride-core CONFIG REQUIRED)
endif()

add_library(cgride_toolchains
  src/toolchain.cpp
  src/discovery.cpp
  src/compile_command.cpp
  src/archive_command.cpp
  src/link_command.cpp
)

add_library(cgride::toolchains ALIAS cgride_toolchains)

target_compile_features(cgride_toolchains
  PUBLIC
    cxx_std_23
)

target_include_directories(cgride_toolchains
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(cgride_toolchains
  PUBLIC
    cgride::core
)

set_target_properties(cgride_toolchains
  PROPERTIES
    OUTPUT_NAME cgride_toolchains
    EXPORT_NAME toolchains
    CXX_EXTENSIONS OFF
)

if(CGRIDE_TOOLCHAINS_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()

if(CGRIDE_TOOLCHAINS_INSTALL)
  install(
    TARGETS cgride_toolchains
    EXPORT cgride-toolchains-targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  )

  install(
    DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  )

  install(
    EXPORT cgride-toolchains-targets
    NAMESPACE cgride::
    FILE cgride-toolchains-targets.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cgride-toolchains
  )

  configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/cgride-toolchains-config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/cgride-toolchains-config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cgride-toolchains
  )

  write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/cgride-toolchains-config-version.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
  )

  install(
    FILES
      ${CMAKE_CURRENT_BINARY_DIR}/cgride-toolchains-config.cmake
      ${CMAKE_CURRENT_BINARY_DIR}/cgride-toolchains-config-version.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cgride-toolchains
  )
endif()
