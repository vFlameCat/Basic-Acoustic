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

  # Third-party code: silence its warnings (the GCC-only -Wno-* list broke MSVC builds).
  # -w is understood by GCC, Clang and MSVC alike.
  target_compile_options(raylib PRIVATE -w)

endif()


# Dear ImGui
CPMAddPackage(
  NAME imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG v1.90.9
  DOWNLOAD_ONLY YES
)

if(imgui_ADDED)

  add_library(imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
  )

  # SYSTEM include silences the project's strict warnings inside imgui headers
  # for downstream consumers (app); PRIVATE -w silences them in imgui's TU.
  target_include_directories(imgui SYSTEM PUBLIC ${imgui_SOURCE_DIR})

  target_compile_options(imgui PRIVATE -w)

endif()


# rlImGui (Dear ImGui <-> raylib glue)
CPMAddPackage(
  NAME rlImGui
  GIT_REPOSITORY https://github.com/raylib-extras/rlImGui.git
  GIT_TAG d765c1ef3d37cf939f88aaa272a59a2713d654c9
  DOWNLOAD_ONLY YES
)

if(rlImGui_ADDED)

  add_library(rlImGui STATIC
    ${rlImGui_SOURCE_DIR}/rlImGui.cpp
  )

  target_include_directories(rlImGui SYSTEM PUBLIC ${rlImGui_SOURCE_DIR})
  target_link_libraries(rlImGui PUBLIC imgui raylib)

  target_compile_options(rlImGui PRIVATE -w)

endif()

