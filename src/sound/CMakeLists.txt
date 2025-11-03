target_sources(${MAIN_TARGET} PRIVATE SoundStorage.cpp)
target_sources(${MAIN_TARGET} PRIVATE AcousticManager.cpp)
target_sources(${MAIN_TARGET} PRIVATE SoundPlayer.cpp)
target_sources(${MAIN_TARGET} PRIVATE PlayCursor.cpp)
target_sources(${MAIN_TARGET} PRIVATE SyncPlayers.cpp)

add_subdirectory(miniaudio)
target_link_libraries(${MAIN_TARGET} PRIVATE miniaudio)

target_include_directories(${MAIN_TARGET} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

