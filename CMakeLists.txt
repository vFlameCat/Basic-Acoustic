cmake_minimum_required(VERSION 3.30)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


project(BasicAcoustic)


# Prevent the user from polluting the root directory with build files
if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(
    FATAL_ERROR
      "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
    )
endif()


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast -Wall -Wextra -Wpedantic -Wno-unused-parameter \
                                        -Wshadow -Wredundant-decls -Wcast-align \
                                        -Wmissing-include-dirs -Weffc++ -Wmain -Wcast-qual \
                                        -Wctor-dtor-privacy -Wformat-security -Wlogical-op \
                                        -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith \
                                        -Wstrict-aliasing -Wstrict-null-sentinel -Wwrite-strings -Wno-comment")


include("cmake/get_cpm.cmake")
include("cmake/thirdparty.cmake")


add_subdirectory(lib)
add_subdirectory(app)

