cmake_minimum_required(VERSION 3.1)

project(Tripe)

## If you want to link SFML statically
# set(SFML_STATIC_LIBRARIES TRUE)

## In most cases better set in the CMake cache
set(SFML_DIR "C:\\Users\\rodri\\programming\\SFML-2.5.1\\lib\\cmake\\SFML")

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

include_directories("C:\\Users\\rodri\\programming\\SFML-2.5.1\\include")
include_directories("C:\\Users\\rodri\\programming\\mingw64\\include")

link_directories("C:\\Users\\rodri\\programming\\mingw64\\lib")
link_directories("C:\\Users\\rodri\\programming\\SFML-2.5.1\\lib")

add_executable(Tripe main.cpp)

target_link_libraries(Tripe sfml-graphics sfml-audio)
