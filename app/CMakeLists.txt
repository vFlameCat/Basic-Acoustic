add_executable(app main.cpp)

target_link_libraries(app game)


add_compile_definitions(
  APP_RESOURCES_ROOT="${PROJECT_SOURCE_DIR}/resources/"
  APP_ROOT="${PROJECT_SOURCE_DIR}/"
)
