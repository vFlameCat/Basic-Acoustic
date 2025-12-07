add_library(miniaudio miniaudio.cpp)
target_compile_options(miniaudio PRIVATE -Wno-unused-result)

target_include_directories(miniaudio PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_options(miniaudio PRIVATE -Wno-conversion -Wno-cast-qual)
