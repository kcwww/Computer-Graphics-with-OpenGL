#include "context.h"
#include "image.h"

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
    // x, y, z, r, g, b, s, t
    0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
    0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
    -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top left
};

  uint32_t indices[] = {
    0, 1, 3, // first triangle
    1, 2, 3, // second triangle
  };

  m_vertexLayout = VertexLayout::Create(); // VAO 생성
  m_vertexBuffer = Buffer::CraeteWithData(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vectices, sizeof(vectices));
  

  // 버텍스 속성 설정
  m_vertexLayout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
  m_vertexLayout->SetAttrib(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 3 * sizeof(float));
  m_vertexLayout->SetAttrib(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 6 * sizeof(float));

  // 인덱스 버퍼 생성
  m_elementBuffer = Buffer::CraeteWithData(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices, sizeof(indices));
  
  // Shader 생성
  ShaderPtr vertexShader = Shader::CreateFromFile("./shader/texture.vs", GL_VERTEX_SHADER);
  ShaderPtr fragmentShader = Shader::CreateFromFile("./shader/texture.fs", GL_FRAGMENT_SHADER);
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
  
  auto image = Image::Load("./image/container.jpg");
  if (!image) return false;
  SPDLOG_INFO("Image size: {} x {}, {} channels", image->GetWidth(), image->GetHeight(), image->GetChannelCount());
  
  glGenTextures(1, &m_texture); // 텍스처 생성
  glBindTexture(GL_TEXTURE_2D, m_texture); // 텍스처 바인딩
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // 텍스처 필터 설정
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // 텍스처 랩 설정

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->GetWidth(), image->GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, image->GetData()); // 텍스처 데이터 설정
  return true;
}

void Context::Render() {
  glClear(GL_COLOR_BUFFER_BIT); //현재 활성화된 프레임 버퍼의 특정 비트 지우기. 화면을 설정된 색상 채움


  m_program->Use(); // 사용할 프로그램을 지정
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
};
