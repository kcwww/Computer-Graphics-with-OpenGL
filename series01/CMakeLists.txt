cmake_minimum_required(VERSION 3.13) # Minimum version of cmake required
set(PROJECT_NAME cmake_project) # Set the project name
set(CMAKE_CXX_STANDARD 17) # Enable c++17 standard

project(${PROJECT_NAME}) # Project Name
add_executable(${PROJECT_NAME} src/main.cpp) # Add an executable
