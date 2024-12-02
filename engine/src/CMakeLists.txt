cmake_minimum_required(VERSION 3.20)
project(Comet)
set(CMAKE_CXX_STANDARD 17)

set(LIBRARY_OUTPUT_PATH ${LIBPATH})

file(GLOB ENGINESRC 
    "./core/*.cpp"
    "./events/*.cpp"
    "./graphics/*.cpp"
)

add_library(engine ${ENGINESRC})
target_link_libraries(engine PUBLIC spdlog glfw ${Vulkan_LIBRARIES})

target_precompile_headers(engine PUBLIC "pch.h")