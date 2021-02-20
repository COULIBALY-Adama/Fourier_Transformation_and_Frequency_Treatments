cmake_minimum_required(VERSION 3.8)
project(Frequences)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Filtres.cpp Frequences.cpp Frequences.h)
add_executable(Frequences ${SOURCE_FILES} Filtres.cpp Frequences.cpp Frequences.h)

include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(Frequences ${OpenCV_LIBS}} )