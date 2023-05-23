cmake_minimum_required(VERSION 3.1)
project(MOUSE)

find_package(OpenCV REQUIRED)

add_executable(mouse mouse.cpp)
target_link_libraries(mouse ${OpenCV_LIBS})