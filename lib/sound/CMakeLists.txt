add_library(sound
    AudioStorage.cpp
    SimulationManager.cpp
    AudioEngine.cpp
    AudioRenderer.cpp

    Players/Player.cpp
    Players/PlayersPool.cpp
    Players/SpatialFramePlayers.cpp
)

target_link_libraries(sound PUBLIC miniaudio vector3 concurrency containers)

target_include_directories(sound PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})