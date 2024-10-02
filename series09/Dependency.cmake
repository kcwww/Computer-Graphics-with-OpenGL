#ExternalProject_Add
include(ExternalProject) # cmake 에서 추가적인 프로젝트를 빌드하는 기능을 제공하는 모듈

# Dependency 관련 변수 설정
set(DEP_INSTALL_DIR ${PROJECT_BINARY_DIR}/install)
set(DEP_INCLUDE_DIR ${DEP_INSTALL_DIR}/include)
set(DEP_LIB_DIR ${DEP_INSTALL_DIR}/lib)

#spdlog 라이브러리 다운로드 및 빌드
ExternalProject_Add(
    dep_spdlog # 대표 이름
    GIT_REPOSITORY "https://github.com/gabime/spdlog.git"
    GIT_TAG "v1.x"
    GIT_SHALLOW 1
    UPDATE_COMMAND ""
    PATCH_COMMAND ""
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${DEP_INSTALL_DIR}
    TEST_COMMAND ""
)

# Dependecy 리스트 및 라이브러리 파일 리스트 추가
set(DEP_LIST ${DEP_LIST} dep_spdlog)
set(DEP_LIBS ${DEP_LIBS} spdlog)


#glfw

ExternalProject_Add(
  dep_glfw
  GIT_REPOSITORY "https://github.com/glfw/glfw.git"
  GIT_TAG "3.4"
  GIT_SHALLOW 1
  UPDATE_COMMAND ""
  PATCH_COMMAND ""
  TEST_COMMAND ""
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${DEP_INSTALL_DIR}
  -DGLFW_BUILD_DOCS=OFF # 문서
  -DGLFW_BUILD_EXAMPLES=OFF # 예제
  -DGLFW_BUILD_TESTS=OFF # 테스트
)

set(DEP_LIST ${DEP_LIST} dep_glfw)
set(DEP_LIBS ${DEP_LIBS} glfw3)


# glad
ExternalProject_Add(
  dep_glad
  GIT_REPOSITORY "https://github.com/Dav1dde/glad.git"
  GIT_TAG "v0.1.34"
  GIT_SHALLOW 1
  UPDATE_COMMAND ""
  PATCH_COMMAND ""
  TEST_COMMAND ""
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${DEP_INSTALL_DIR}
  -DGLAD_INSTALL=ON
)

set(DEP_LIST ${DEP_LIST} dep_glad)
set(DEP_LIBS ${DEP_LIBS} glad)

# stb
ExternalProject_Add(
  dep_stb
  GIT_REPOSITORY "https://github.com/nothings/stb.git"
  GIT_TAG "master"
  GIT_SHALLOW 1
  UPDATE_COMMAND ""
  PATCH_COMMAND ""
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  TEST_COMMAND ""
  INSTALL_COMMAND ${CMAKE_COMMAND} -E copy
      ${PROJECT_BINARY_DIR}/dep_stb-prefix/src/dep_stb/stb_image.h
      ${DEP_INSTALL_DIR}/include/stb/stb_image.h
)

set(DEP_LIST ${DEP_LIST} dep_stb)

#glm
ExternalProject_Add(
  dep_glm
  GIT_REPOSITORY "https://github.com/g-truc/glm.git"
  GIT_TAG "1.0.1"
  GIT_SHALLOW 1
  UPDATE_COMMAND ""
  PATCH_COMMAND ""
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  TEST_COMMAND ""
  INSTALL_COMMAND ${CMAKE_COMMAND} -E copy_directory
      ${PROJECT_BINARY_DIR}/dep_glm-prefix/src/dep_glm/glm
      ${DEP_INSTALL_DIR}/include/glm
)

set(DEP_LIST ${DEP_LIST} dep_glm)


add_library(imgui
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/imgui.cpp
    imgui/imgui_impl_glfw.cpp
    imgui/imgui_impl_opengl3.cpp
    )
target_include_directories(imgui PRIVATE ${DEP_INCLUDE_DIR})
add_dependencies(imgui ${DEP_LIST})
set(DEP_INCLUDE_DIR ${DEP_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
set(DEP_LIST ${DEP_LIST} imgui)
set(DEP_LIBS ${DEP_LIBS} imgui)

# assimp
ExternalProject_Add(
    dep_assimp
    GIT_REPOSITORY "https://github.com/assimp/assimp"
    GIT_TAG "v5.0.1"
    GIT_SHALLOW 1
    UPDATE_COMMAND ""
    PATCH_COMMAND ""
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${DEP_INSTALL_DIR}
        -DBUILD_SHARED_LIBS=OFF
        -DASSIMP_BUILD_ASSIMP_TOOLS=OFF
        -DASSIMP_BUILD_TESTS=OFF
        -DASSIMP_INJECT_DEBUG_POSTFIX=OFF
        -DASSIMP_BUILD_ZLIB=ON
    TEST_COMMAND ""
    )
set(DEP_LIST ${DEP_LIST} dep_assimp)
set(DEP_LIBS ${DEP_LIBS}
    assimp
    zlibstatic
    IrrXML
    )
