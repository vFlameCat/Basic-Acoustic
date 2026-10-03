add_library(rta_sound STATIC
    AudioStorage.cpp
    SimulationManager.cpp
    AudioEngine.cpp
    AudioRenderer.cpp

    Players/Player.cpp
    Players/PlayersPool.cpp
    Players/SpatialFramePlayers.cpp
)

add_library(rta::sound ALIAS rta_sound)

target_link_libraries(rta_sound PUBLIC rta_miniaudio rta_math rta_concurrency rta_containers)

target_include_directories(rta_sound PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_features(rta_sound PUBLIC cxx_std_20)

# Allows linking the static library into a shared one (e.g. an engine module)
set_target_properties(rta_sound PROPERTIES POSITION_INDEPENDENT_CODE ON)

rta_set_compile_options(rta_sound)
