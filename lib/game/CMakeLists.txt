add_library(game 
    Game.cpp
    Scene.cpp
    sound/SoundStorage.cpp
    sound/AcousticManager.cpp
    sound/SoundPlayer.cpp
    sound/PlayCursor.cpp
    sound/SyncPlayers.cpp
    sound/AudioRenderer.cpp
)

target_link_libraries(game PUBLIC miniaudio vector3)

target_include_directories(game PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(game PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/sound)