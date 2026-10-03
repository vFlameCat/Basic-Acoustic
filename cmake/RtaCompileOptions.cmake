# Warning flags for Basic-Acoustic's own targets (library and demo).
# Third-party code (miniaudio, raylib, imgui) is configured separately.
function(rta_set_compile_options target)

  if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")

    target_compile_options(${target} PRIVATE
      -Wall -Wextra -Wpedantic -Wno-unused-parameter
      -Wshadow -Wredundant-decls -Wcast-align
      -Wmissing-include-dirs -Weffc++ -Wmain -Wcast-qual
      -Wctor-dtor-privacy -Wformat-security -Wlogical-op
      -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith
      -Wstrict-aliasing -Wstrict-null-sentinel -Wwrite-strings -Wno-comment
    )

  endif()

endfunction()
