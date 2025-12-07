add_library(vector3 INTERFACE)

target_link_libraries(vector3 INTERFACE raylib)

target_include_directories(vector3 INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}) 