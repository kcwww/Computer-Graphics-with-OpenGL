#include "context.h"

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

//imgui
void OnCharEvent (GLFWwindow* window, unsigned int ch) {
    ImGui_ImplGlfw_CharCallback(window, ch);
}

void OnScroll(GLFWwindow* window, double x, double y) {
    ImGui_ImplGlfw_ScrollCallback(window, x, y);
}

void OnFrameBufferSizeChange(GLFWwindow* window, int width, int height) {
    SPDLOG_INFO("Frame Buffer Size Changed : {} x {}", width, height);
    // glViewport 함수를 통해 OpenGL의 렌더링 영역을 설정
    // glViewport(0, 0, width, height);

    // reshape
    auto context = reinterpret_cast<Context*>(glfwGetWindowUserPointer(window));
    context->Reshape(width, height);
}

void OnCursorPos(GLFWwindow* window, double x, double y) {
    auto context = reinterpret_cast<Context*>(glfwGetWindowUserPointer(window));
    context->MoveMouse(x, y);
}

void OnMouseButton(GLFWwindow* window, int button, int action, int mods) {
    // imgui mouse button
    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);

    auto context = reinterpret_cast<Context*>(glfwGetWindowUserPointer(window));
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    context->MouseButton(button, action, x, y);
}

void OnKeyEvent(GLFWwindow *window, int key, int scancode, int action, int mods) {
    // imgui key event
    ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);

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
    
    // imgui 초기화
    auto imguiContext = ImGui::CreateContext();
    ImGui::SetCurrentContext(imguiContext);
    ImGui_ImplGlfw_InitForOpenGL(window, false); // callback 함수를 직접 호출 = false
    ImGui_ImplOpenGL3_Init();
    ImGui_ImplOpenGL3_CreateFontsTexture(); // font texture 생성
    ImGui_ImplOpenGL3_CreateDeviceObjects(); // device object 생성

    // Context 생성
    auto context = Context::Create();
    if (!context) {
        SPDLOG_ERROR("Failed to create context");
        glfwTerminate();
        return -1;
    }

    // user pointer 설정
    glfwSetWindowUserPointer(window, context.get());

    // 프레임 버퍼 사이즈 변경 콜백 함수 등록
    glfwSetFramebufferSizeCallback(window, OnFrameBufferSizeChange);
    glfwSetKeyCallback(window, OnKeyEvent);

    // mouse callback
    glfwSetCursorPosCallback(window, OnCursorPos);
    glfwSetMouseButtonCallback(window, OnMouseButton);
    // imgui mouse scroll
    glfwSetScrollCallback(window, OnScroll);
    // imgui char event
    glfwSetCharCallback(window, OnCharEvent);

    // glfw 루프
    SPDLOG_INFO("Start GLFW Loop");
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // imgui new frame
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        context->ProcessInput(window); // 입력 처리 함수
        context->Render();

        // imgui render
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window); // 이전 버퍼와 다음 버퍼를 교체
    }
    context.reset();

    // imgui 종료
    ImGui_ImplOpenGL3_DestroyDeviceObjects();
    ImGui_ImplOpenGL3_DestroyFontsTexture();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext(imguiContext);

    glfwTerminate();
    return 0;
}
