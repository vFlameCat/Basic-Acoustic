# Warning settings for Basic-Acoustic's own targets (library and demo).
# Third-party code (miniaudio, raylib, imgui) is configured separately.
function(rta_set_compile_options target)

  if(MSVC)

    # /wd4100 (unreferenced parameter) mirrors -Wno-unused-parameter of GCC/Clang.
    # /utf-8: sources are UTF-8, otherwise MSVC reads them in the system code page.
    set(warnings /W4 /permissive- /wd4100 /utf-8)
    set(warningsAsErrors /WX)

  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")

    set(warnings
      -Wall -Wextra -Wpedantic -Wno-unused-parameter
      -Wshadow -Wredundant-decls -Wcast-align
      -Wmissing-include-dirs -Weffc++ -Wmain -Wcast-qual
      -Wctor-dtor-privacy -Wformat-security -Wlogical-op
      -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith
      -Wstrict-aliasing -Wstrict-null-sentinel -Wwrite-strings -Wno-comment
    )
    set(warningsAsErrors -Werror)

  elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")

    # Same as GCC without the GCC-only -Wlogical-op and -Wstrict-null-sentinel
    set(warnings
      -Wall -Wextra -Wpedantic -Wno-unused-parameter
      -Wshadow -Wredundant-decls -Wcast-align
      -Wmissing-include-dirs -Weffc++ -Wmain -Wcast-qual
      -Wctor-dtor-privacy -Wformat-security
      -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith
      -Wstrict-aliasing -Wwrite-strings -Wno-comment
    )
    set(warningsAsErrors -Werror)

  endif()

  target_compile_options(${target} PRIVATE ${warnings})

  # Only RTA_WARNINGS_AS_ERRORS decides whether these warnings are errors:
  # a parent project's CMAKE_COMPILE_WARNING_AS_ERROR is not applied to this code.
  set_target_properties(${target} PROPERTIES COMPILE_WARNING_AS_ERROR OFF)

  if(RTA_WARNINGS_AS_ERRORS)
    target_compile_options(${target} PRIVATE ${warningsAsErrors})
  endif()

endfunction()
