cmake_minimum_required(VERSION 2.8)

project(old-classic-minesweeper)

add_executable(${PROJECT_NAME} "main.cpp")

target_link_libraries(${PROJECT_NAME} glut GL)
