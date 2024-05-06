cmake_minimum_required(VERSION 3.0.0)
project(Wyvern VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

add_executable(Wyvern main.cpp)

target_include_directories(Wyvern PUBLIC C:/Pleiades/vcpkg/installed/x86-windows/include)
target_link_directories(Wyvern PUBLIC C:/Pleiades/vcpkg/installed/x86-windows/lib)

find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
target_link_libraries(Wyvern PRIVATE sfml-system sfml-network sfml-graphics sfml-window OpenGL::GL)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
