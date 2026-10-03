include(${PROJECT_SOURCE_DIR}/cmake/get_cpm.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/thirdparty.cmake)


add_executable(app
    main.cpp
    Game.cpp
    Scene.cpp
)

target_link_libraries(app PRIVATE rta::sound rlImGui)

target_compile_definitions(app PRIVATE
    APP_RESOURCES_ROOT="${PROJECT_SOURCE_DIR}/resources/"
)

rta_set_compile_options(app)
