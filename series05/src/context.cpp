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
  m_vertexLayout->SetAttrib(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 6 * sizeof(float));

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
  // auto image = Image::Create(512,512);
  // image->SetCheckImage(16, 16);
  
  if (!image) return false;
  SPDLOG_INFO("Image size: {} x {}, {} channels", image->GetWidth(), image->GetHeight(), image->GetChannelCount());
  m_texture = Texture::CreateFromImage(image.get());
  
  
  auto image2 = Image::Load("./image/awesomeface.png");
  m_texture2 = Texture::CreateFromImage(image2.get());

  // 텍스처 바인딩 및 설정 슬롯 0, 1
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_texture->Get());
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, m_texture2->Get());

  // 프로그램에 텍스처 유니폼 설정
  m_program->Use();
  glUniform1i(glGetUniformLocation(m_program->Get(), "tex"), 0);
  glUniform1i(glGetUniformLocation(m_program->Get(), "tex2"), 1);
  
  return true;
}

void Context::Render() {
  glClear(GL_COLOR_BUFFER_BIT); //현재 활성화된 프레임 버퍼의 특정 비트 지우기. 화면을 설정된 색상 채움


  m_program->Use(); // 사용할 프로그램을 지정
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
};
