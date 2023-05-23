cmake_minimum_required(VERSION 3.10)

project(MAIN)

add_executable(main_cmake main.cpp)

find_package( OpenCV REQUIRED )

target_link_libraries( MAIN ${OpenCV_LIBS} )