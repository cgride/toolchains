function(cgride_toolchains_add_test name source)
  add_executable(${name}
    ${source}
  )

  target_link_libraries(${name}
    PRIVATE
      cgride::toolchains
  )

  target_compile_features(${name}
    PRIVATE
      cxx_std_23
  )

  set_target_properties(${name}
    PROPERTIES
      CXX_EXTENSIONS OFF
  )

  add_test(
    NAME ${name}
    COMMAND ${name}
  )
endfunction()

cgride_toolchains_add_test(cgride_toolchains_toolchain_test toolchain_test.cpp)
cgride_toolchains_add_test(cgride_toolchains_discovery_test discovery_test.cpp)
cgride_toolchains_add_test(cgride_toolchains_compile_command_test compile_command_test.cpp)
cgride_toolchains_add_test(cgride_toolchains_archive_command_test archive_command_test.cpp)
cgride_toolchains_add_test(cgride_toolchains_link_command_test link_command_test.cpp)
