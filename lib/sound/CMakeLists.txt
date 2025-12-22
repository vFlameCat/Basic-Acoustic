add_library(sound 
    AudioStorage.cpp
    SimulationManager.cpp
    AudioPlayer.cpp
    PlayCursor.cpp
    SyncPlayers.cpp
    AudioRenderer.cpp
    AudioSourcesStorage.cpp
)

target_link_libraries(sound PUBLIC miniaudio vector3)

target_include_directories(sound PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})