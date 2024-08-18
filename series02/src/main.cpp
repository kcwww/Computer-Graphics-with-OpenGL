#include <spdlog/spdlog.h>
#include <GLFW/glfw3.h>

int main() {
    SPDLOG_INFO("Start Program");

    //glfw 라이브러리 초기화
    if (!glfwInit()) {
        const char * description = nullptr;
        glfwGetError(&description);
        SPDLOG_ERROR("Failed to initialize GLFW : {}", description);
        return -1;
    }

    //glfw 윈도우 생성
    SPDLOG_INFO("Create GLFW Window");
    auto window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, nullptr, nullptr);
    if (!window) {
        SPDLOG_ERROR("Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }

    // glfw 루프
    SPDLOG_INFO("Start GLFW Loop");
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
