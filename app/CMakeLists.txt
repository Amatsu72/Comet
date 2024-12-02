cmake_minimum_required(VERSION 3.20)
project(Comet)
set(CMAKE_CXX_STANDARD 17)

set(EXECUTABLE_OUTPUT_PATH ${OUTPATH})

file(GLOB SOURCES "*.cpp")
# file(GLOB SOURCES "*.cpp" ${3RDPATH}/stb_image/*.cpp)
add_executable(app ${SOURCES})
target_link_libraries(app PUBLIC engine) 