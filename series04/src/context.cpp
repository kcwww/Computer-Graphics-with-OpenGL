#include "context.h"

ContextUPtr Context::Create() {
  auto context = ContextUPtr(new Context());
  if (!context->Init()) {
    return nullptr;
  }
  return std::move(context);
}

bool Context::Init() {
  // Shader 생성
  ShaderPtr vertexShader = Shader::CreateFromFile("./shader/simple.vs", GL_VERTEX_SHADER);
  ShaderPtr fragmentShader = Shader::CreateFromFile("./shader/simple.fs", GL_FRAGMENT_SHADER);
  SPDLOG_INFO("vertexShader : {}", vertexShader->Get());
  SPDLOG_INFO("fragmentShader : {}", fragmentShader->Get());
  if (!vertexShader || !fragmentShader) {
    return false;
  }
  // Program 생성
  m_program = Program::Create({vertexShader, fragmentShader});
  if (!m_program) {
    return false;
  }
  SPDLOG_INFO("program : {}", m_program->Get());
  glClearColor(0.1f, 0.2f, 0.3f, 0.0f); //화면을 지울 때 사용할 배경색을 설정 RGB 투명도
  return true;
}

void Context::Render() {
  glClear(GL_COLOR_BUFFER_BIT); //현재 활성화된 프레임 버퍼의 특정 비트 지우기. 화면을 설정된 색상 채움
};
