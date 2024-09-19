cmake_minimum_required(VERSION 3.13) # Minimum version of cmake required
set(PROJECT_NAME SERIES04) # Set the project name
set(CMAKE_CXX_STANDARD 17) # Enable c++17 standard

set(WINDOW_NAME "OpenGL Example") # Set the window name
set(WINDOW_WIDTH 960)
set(WINDOW_HEIGHT 540)

project(${PROJECT_NAME}) # Project Name
add_executable(${PROJECT_NAME}
    src/main.cpp
    src/common.cpp src/common.h
    src/shader.cpp src/shader.h
    src/program.cpp src/program.h
    src/context.h src/context.cpp
    src/buffer.h src/buffer.cpp
    src/vertex_layout.h src/vertex_layout.cpp
) # Add an executab le

include(Dependency.cmake)

# 우리 프로젝트에 include / lib 관련 옵션 추가
target_include_directories(${PROJECT_NAME} PUBLIC ${DEP_INCLUDE_DIR})
target_link_directories(${PROJECT_NAME} PUBLIC ${DEP_LIB_DIR})
# target_link_libraries(${PROJECT_NAME} PUBLIC ${DEP_LIBS})

# 필요한 macOS 프레임워크를 추가합니다.
target_link_libraries(${PROJECT_NAME} PUBLIC ${DEP_LIBS}
    "-framework Cocoa"        # Cocoa 프레임워크 (Objective-C 런타임)
    "-framework IOKit"        # IOKit 프레임워크 (하드웨어 관련 API)
    "-framework CoreFoundation" # CoreFoundation 프레임워크 (기본 데이터 타입과 유틸리티)
)

target_compile_definitions(${PROJECT_NAME} PUBLIC
    WINDOW_NAME="${WINDOW_NAME}"
    WINDOW_WIDTH=${WINDOW_WIDTH}
    WINDOW_HEIGHT=${WINDOW_HEIGHT}
)

# Dependency 빌드 순서 설정
add_dependencies(${PROJECT_NAME} ${DEP_LIST})
