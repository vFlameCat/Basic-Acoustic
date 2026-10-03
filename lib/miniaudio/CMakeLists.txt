add_library(rta_miniaudio STATIC miniaudio.cpp)

# Third-party code: keep its warnings out of our builds (and out of a parent's warnings-as-errors)
target_compile_options(rta_miniaudio PRIVATE $<IF:$<CXX_COMPILER_ID:MSVC>,/w,-w>)
set_target_properties(rta_miniaudio PROPERTIES COMPILE_WARNING_AS_ERROR OFF)

target_include_directories(rta_miniaudio SYSTEM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# miniaudio needs pthread and dl (plus libm on Linux/BSD); on Windows nothing extra is linked
find_package(Threads REQUIRED)
target_link_libraries(rta_miniaudio PRIVATE Threads::Threads ${CMAKE_DL_LIBS})

if(UNIX AND NOT APPLE)
  target_link_libraries(rta_miniaudio PRIVATE m)
endif()

# Allows linking the static library into a shared one (e.g. an engine module)
set_target_properties(rta_miniaudio PROPERTIES POSITION_INDEPENDENT_CODE ON)
