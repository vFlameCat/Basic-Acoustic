cmake_minimum_required(VERSION 3.30)

# raylib
CPMAddPackage(
  NAME raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib.git
  GIT_TAG 5.5
  OPTIONS
    "BUILD_EXAMPLES OFF"
    "BUILD_GAMES OFF"
    "SHARED OFF"
)

if(raylib_ADDED AND TARGET raylib)

  target_compile_options(raylib PRIVATE
    -Wno-unused-parameter
    -Wno-unused-result
    -Wno-implicit-fallthrough
    -Wno-stringop-overflow
    -Wno-stringop-truncation
    -Wno-sign-compare
  )

endif()

