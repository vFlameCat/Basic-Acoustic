add_library(game 
    Game.cpp
    Scene.cpp
)

target_link_libraries(game PUBLIC sound rlImGui)

target_include_directories(game PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})