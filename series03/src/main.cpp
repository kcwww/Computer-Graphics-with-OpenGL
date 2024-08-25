#include "common.h"
#include "shader.h"

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

void Render() {
    glClearColor(0.1f, 0.2f, 0.3f, 0.0f); //화면을 지울 때 사용할 배경색을 설정 RGB 투명도
    glClear(GL_COLOR_BUFFER_BIT); //현재 활성화된 프레임 버퍼의 특정 비트 지우기. 화면을 설정된 색상 채움
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
    
    
    /* 여기까지 초기화 및 세팅 후 OpenGL function 을 사용할 수 있다  */

    // Shader 생성
    auto vertexShader = Shader::CreateFromFile("./shader/simple.vs", GL_VERTEX_SHADER);
    auto fragmentShader = Shader::CreateFromFile("./shader/simple.fs", GL_FRAGMENT_SHADER);
    SPDLOG_INFO("vertexShader : {}", vertexShader->Get());
    SPDLOG_INFO("fragmentShader : {}", fragmentShader->Get());


    // 프레임 버퍼 사이즈 변경 콜백 함수 등록
    glfwSetFramebufferSizeCallback(window, OnFrameBufferSizeChange);
    glfwSetKeyCallback(window, OnKeyEvent);
    // glfw 루프
    SPDLOG_INFO("Start GLFW Loop");
    while (!glfwWindowShouldClose(window)) {
        Render();
        glfwSwapBuffers(window); // 이전 버퍼와 다음 버퍼를 교체
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
