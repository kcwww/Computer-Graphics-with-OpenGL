#include "context.h"

void OnFrameBufferSizeChange(GLFWwindow* window, int width, int height) {
    SPDLOG_INFO("Frame Buffer Size Changed : {} x {}", width, height);
    // glViewport 함수를 통해 OpenGL의 렌더링 영역을 설정
    glViewport(0, 0, width, height);
}

void OnKeyEvent(GLFWwindow *window, int key, int scancode, int action, int mods) {

    SPDLOG_INFO("key : {}, scancode : {}, action : {}, mods : {}{}{}",
    key, scancode,
    action == GLFW_PRESS ? "Pressed" :
    action == GLFW_RELEASE ? "Released" :
    action == GLFW_REPEAT ? "Repeated" : "Unknown",
    mods & GLFW_MOD_CONTROL ? "C" : "-",
    mods & GLFW_MOD_SHIFT ? "S" : "-",
    mods & GLFW_MOD_ALT ? "A" : "-"
    );

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}



int main() {
    SPDLOG_INFO("Start Program");

    //glfw 라이브러리 초기화
    if (!glfwInit()) {
        const char * description = nullptr;
        glfwGetError(&description);
        SPDLOG_ERROR("Failed to initialize GLFW : {}", description);
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //glfw 윈도우 생성
    SPDLOG_INFO("Create GLFW Window");
    auto window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, nullptr, nullptr);
    if (!window) {
        SPDLOG_ERROR("Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // glad를 활용한 OpenGL 함수 로딩
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        SPDLOG_ERROR("Failed to initialize GLAD");
        return -1;
    }
    const char* glVersion = (const char*)glGetString(GL_VERSION);
    SPDLOG_INFO("OpenGL Version : {}", glVersion);
    
    // Context 생성
    auto context = Context::Create();
    if (!context) {
        SPDLOG_ERROR("Failed to create context");
        glfwTerminate();
        return -1;
    }


    // 프레임 버퍼 사이즈 변경 콜백 함수 등록
    glfwSetFramebufferSizeCallback(window, OnFrameBufferSizeChange);
    glfwSetKeyCallback(window, OnKeyEvent);
    // glfw 루프
    SPDLOG_INFO("Start GLFW Loop");
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        context->Render();
        glfwSwapBuffers(window); // 이전 버퍼와 다음 버퍼를 교체
    }
    context.reset();
    glfwTerminate();
    return 0;
}
