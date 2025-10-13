target_sources(${MAIN_TARGET} PRIVATE main.cpp)
target_sources(${MAIN_TARGET} PRIVATE Game.cpp)
target_sources(${MAIN_TARGET} PRIVATE Scene.cpp)
target_sources(${MAIN_TARGET} PRIVATE SampleManager.cpp)
target_sources(${MAIN_TARGET} PRIVATE AcousticManager.cpp)
target_sources(${MAIN_TARGET} PRIVATE SceneSoundPlayer.cpp)

add_subdirectory(miniaudio)
target_link_libraries(${MAIN_TARGET} PRIVATE miniaudio)
