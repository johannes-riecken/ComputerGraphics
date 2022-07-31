cmake_minimum_required(VERSION 3.10)

# set the project name
project(ComputerGraphics)

find_path(GLEW_INCLUDE_DIR GL/glew.h)

find_library(GLEW_LIBRARY GLEW PATH_SUFFIXES lib64)
find_library(FREEGLUT3_LIBRARY freeglut3)
find_library(Z_LIBRARY z)
find_library(JPEG_LIBRARY jpeg)
find_library(PNG_LIBRARY png)

# add the executable
add_executable(wood src/wood.cpp)

include_directories(${GLEW_INCLUDE_DIR})

target_link_libraries(wood PUBLIC ${GLEW_LIBRARY} ${FREEGLUT_LIBRARY} ${Z_LIBRARY} ${JPEG_LIBRARY} ${PNG_LIBRARY} pthread)

target_compile_features(wood PUBLIC cxx_std_17)
