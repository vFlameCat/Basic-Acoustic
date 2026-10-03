cmake_minimum_required(VERSION 3.21)


project(BasicAcoustic LANGUAGES CXX)


if(PROJECT_IS_TOP_LEVEL)

  # Prevent the user from polluting the root directory with build files
  if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(
      FATAL_ERROR
        "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
      )
  endif()

  # Optimized build unless another build type is requested (single-config generators only)
  get_property(isMultiConfig GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
  if(NOT isMultiConfig AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
  endif()

endif()


# The demo is built only when Basic-Acoustic is the top-level project,
# so a parent project gets just the library and nothing is downloaded.
option(RTA_BUILD_DEMO "Build the raylib demo application" ${PROJECT_IS_TOP_LEVEL})

# The library has its own warnings-as-errors switch; a parent project's
# CMAKE_COMPILE_WARNING_AS_ERROR does not apply to Basic-Acoustic targets.
option(RTA_WARNINGS_AS_ERRORS "Treat warnings in Basic-Acoustic code as errors" OFF)


include("cmake/RtaCompileOptions.cmake")


add_subdirectory(lib)

if(RTA_BUILD_DEMO)
  add_subdirectory(app)
endif()
