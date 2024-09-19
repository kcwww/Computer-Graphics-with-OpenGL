#include "context.h"

ContextUPtr Context::Create() {
  auto context = ContextUPtr(new Context());
  if (!context->Init()) {
    return nullptr;
  }
  return std::move(context);
}

bool Context::Init() {
  // 임시 정점 데이터
  float vectices[] = {
    // first triangle
    0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // top right, red
    0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom right green
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom left blue
    -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, // top left yellow
  };

  uint32_t indices[] = {
    0, 1, 3, // first triangle
    1, 2, 3, // second triangle
  };

  m_vertexLayout = VertexLayout::Create(); // VAO 생성
  m_vertexBuffer = Buffer::CraeteWithData(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vectices, sizeof(float) * 24);
  

  // 버텍스 속성 설정
  m_vertexLayout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
  m_vertexLayout->SetAttrib(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 3 * sizeof(float));
  
  // 인덱스 버퍼 생성
  m_elementBuffer = Buffer::CraeteWithData(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices, sizeof(indices));
  
  // Shader 생성
  ShaderPtr vertexShader = Shader::CreateFromFile("./shader/per_vertex_color.vs", GL_VERTEX_SHADER);
  ShaderPtr fragmentShader = Shader::CreateFromFile("./shader/per_vertex_color.fs", GL_FRAGMENT_SHADER);
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

  // uniform 변수 전달
  // auto loc = glGetUniformLocation(m_program->Get(), "color"); // 프로그램에서 uniform 변수의 위치를 가져옴
  // m_program->Use(); // 사용할 프로그램을 지정
  // glUniform4f(loc, 1.0f, 1.0f, 0.0f, 1.0f); // uniform 변수에 값을 전달


  glClearColor(0.1f, 0.2f, 0.3f, 0.0f); //화면을 지울 때 사용할 배경색을 설정 RGB 투명도
  return true;
}

void Context::Render() {
  glClear(GL_COLOR_BUFFER_BIT); //현재 활성화된 프레임 버퍼의 특정 비트 지우기. 화면을 설정된 색상 채움

  // 색상이 변하는 로직
  // static float time = 0.0f;
  // float t = sinf(time) * 0.5f + 0.5f; // -1 ~ 1 사이의 값이 sin 함수를 통해 나오므로 0 ~ 1로 변환
  // auto loc = glGetUniformLocation(m_program->Get(), "color");
  // m_program->Use(); // 사용할 프로그램을 지정
  // glUniform4f(loc, t*t, 2.0f*t*(1.0f-t), (1.0f-t)*(1.0f-t), 1.0f); // uniform 변수에 값을 전달
  // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  // time += 0.08f;

  m_program->Use(); // 사용할 프로그램을 지정
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
};
