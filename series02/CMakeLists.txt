cmake_minimum_required(VERSION 3.13) # Minimum version of cmake required
set(PROJECT_NAME SERIES02) # Set the project name
set(CMAKE_CXX_STANDARD 17) # Enable c++17 standard

project(${PROJECT_NAME}) # Project Name
add_executable(${PROJECT_NAME} src/main.cpp) # Add an executable

include(Dependency.cmake)

# 우리 프로젝트에 include / lib 관련 옵션 추가
target_include_directories(${PROJECT_NAME} PUBLIC ${DEP_INCLUDE_DIR})
target_link_directories(${PROJECT_NAME} PUBLIC ${DEP_LIB_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC ${DEP_LIBS})

# Dependency 빌드 순서 설정
add_dependencies(${PROJECT_NAME} ${DEP_LIST})
