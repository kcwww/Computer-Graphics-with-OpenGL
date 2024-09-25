#include "context.h"

ContextUPtr Context::Create() {
  auto context = ContextUPtr(new Context());
  if (!context->Init()) {
    return nullptr;
  }
  return std::move(context);
}

bool Context::Init() {
//   // 임시 정점 데이터
//   float vectices[] = {
//     // x, y, z, r, g, b, s, t
//     0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
//     0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
//     -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
//     -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top left
// };

//   uint32_t indices[] = {
//     0, 1, 3, // first triangle
//     1, 2, 3, // second triangle
//   };


  float vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,

         0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,

        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 0.0f,
    };

    uint32_t indices[] = {
         0,  2,  1,  2,  0,  3,
         4,  5,  6,  6,  7,  4,
         8,  9, 10, 10, 11,  8,
        12, 14, 13, 14, 12, 15,
        16, 17, 18, 18, 19, 16,
        20, 22, 21, 22, 20, 23,
    };



  m_vertexLayout = VertexLayout::Create(); // VAO 생성
  m_vertexBuffer = Buffer::CraeteWithData(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices, sizeof(vertices));
  

  // 버텍스 속성 설정
  m_vertexLayout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
  // m_vertexLayout->SetAttrib(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 3 * sizeof(float));
  m_vertexLayout->SetAttrib(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 3 * sizeof(float));

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
  m_program->SetUniform("tex", 0);
  m_program->SetUniform("tex2", 1);

  // // 위치 (1, 0, 0) 의 점. 동차 좌표계 사용
  // glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
  // // 단위 행렬 기준 (1, 1, 0) 만큼 평행 이동 하는 행렬
  // auto trans = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 0.0f));
  // // 단위 행렬 기준 z 축으로 90 도 만큼 회전하는 행렬
  // auto rot = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
  // // 단위행렬 기준 모든 축에 대해 3배를 확대하는 행렬
  // auto scale = glm::scale(glm::mat4(1.0f), glm::vec3(3.0f));
  // // 확대 -> 회전 -> 평행 이동 순으로 점에 선형 변환 적용
  // vec = trans * rot * scale * vec;
  // SPDLOG_INFO("vec: ({}, {}, {})", vec.x, vec.y, vec.z);

  // 0.5배 축소를 z 축으로 90도 회전하는 행렬
  // auto transform = glm::rotate(glm::scale(glm::mat4(1.0f), glm::vec3(0.5f)), glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

  // x 축으로 -55 도 회전
  auto model = glm::rotate(glm::mat4(1.0f), glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
  // 카메라는 원점으로부터 z 축 방향으로 -3 만큼 떨어짐
  auto view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));  
  // 종횡비 4:3, 세로 화각 45도의 원근 투영
  auto projection = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.01f, 10.0f);
  auto transform = projection * view * model;

  m_program->SetUniform("transform", transform);

  return true;
}

// void Context::Render() {
//   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //현재 활성화된 프레임 버퍼의 특정 비트 지우기. 화면을 설정된 색상 채움
//   glEnable(GL_DEPTH_TEST); // 깊이 테스트 활성화

//   m_program->Use(); // 사용할 프로그램을 지정


//   auto projection = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.01f, 10.0f);
//   auto view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
//   auto model = glm::rotate(glm::mat4(1.0f), glm::radians((float)glfwGetTime() * 120.0f), glm::vec3(1.0f, 0.5f, 0.0f));
//   auto transform = projection * view * model;
//   m_program->SetUniform("transform", transform);
//   glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

//   // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
// };


// multiple cube
void Context::Render() {
    std::vector<glm::vec3> cubePositions = {
        glm::vec3( 0.0f, 0.0f, 0.0f),
        glm::vec3( 2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f, 2.0f, -2.5f),
        glm::vec3( 1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f),
    };

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    auto projection = glm::perspective(glm::radians(45.0f),
        (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.01f, 20.0f);
    auto view  = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));

    for (size_t i = 0; i < cubePositions.size(); i++){
        auto& pos = cubePositions[i];
        auto model = glm::translate(glm::mat4(1.0f), pos);
        model = glm::rotate(model,
            glm::radians((float)glfwGetTime() * 120.0f + 20.0f * (float)i),
            glm::vec3(1.0f, 0.5f, 0.0f));
        auto transform = projection * view * model;
        m_program->SetUniform("transform", transform);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }
}
