#include "context.h"
#include <imgui.h>

ContextUPtr Context::Create() {
  auto context = ContextUPtr(new Context());
  if (!context->Init()) {
    return nullptr;
  }
  return std::move(context);
}

bool Context::Init() {
  // 버텍스 데이터
  float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
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
  m_vertexLayout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
  m_vertexLayout->SetAttrib(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 3 * sizeof(float));
  m_vertexLayout->SetAttrib(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 6 * sizeof(float));

  // 인덱스 버퍼 생성
  m_elementBuffer = Buffer::CraeteWithData(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices, sizeof(indices));
  
  // Shader 생성
  ShaderPtr vertexShader = Shader::CreateFromFile("./shader/lighting.vs", GL_VERTEX_SHADER);
  ShaderPtr fragmentShader = Shader::CreateFromFile("./shader/lighting.fs", GL_FRAGMENT_SHADER);
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
  // pos
  


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



void Context::Render() {

    // light parameter
    if (ImGui::Begin("ui window")) {
        if (ImGui::CollapsingHeader("light", ImGuiTreeNodeFlags_DefaultOpen)) {
          ImGui::DragFloat3("light pos", glm::value_ptr(m_lightPos), 0.01f);
          ImGui::ColorEdit3("light color", glm::value_ptr(m_lightColor));
          ImGui::ColorEdit3("object color", glm::value_ptr(m_objectColor));
          ImGui::SliderFloat("ambient strength", &m_ambientStrength, 0.0f, 1.0f);
          ImGui::SliderFloat("specular strength", &m_specularStrength, 0.0f, 1.0f);
          ImGui::DragFloat("shininess", &m_specularShininess, 1.0f, 1.0f, 256.0f);
        }
        // animation
        ImGui::Checkbox("animation", &m_animation);
    }
    ImGui::End();

    
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


    auto projection = glm::perspective(glm::radians(45.0f), (float)m_width / (float)m_height, 0.01f, 30.0f);
    
    m_cameraFront = 
        glm::rotate(glm::mat4(1.0f), glm::radians(m_cameraYaw), glm::vec3(0.0f, 1.0f, 0.0f)) *
        glm::rotate(glm::mat4(1.0f), glm::radians(m_cameraPitch), glm::vec3(1.0f, 0.0f, 0.0f)) *
        glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);

    auto view = glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);


    // light model transform
    auto lightModelTransform =
      glm::translate(glm::mat4(1.0), m_lightPos) *
      glm::scale(glm::mat4(1.0), glm::vec3(0.1f));
    m_program->Use();
    m_program->SetUniform("lightPos", m_lightPos);
    m_program->SetUniform("lightColor", glm::vec3(1.0f));
    m_program->SetUniform("objectColor", glm::vec3(1.0f));
    m_program->SetUniform("ambientStrength", 1.0f);
    m_program->SetUniform("transform", projection * view * lightModelTransform);
    m_program->SetUniform("modelTransform", lightModelTransform);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    // progrma 사용
    m_program->Use();
    m_program->SetUniform("lightColor", m_lightColor);
    m_program->SetUniform("objectColor", m_objectColor);
    m_program->SetUniform("ambientStrength", m_ambientStrength);
    // lightPos
    m_program->SetUniform("lightPos", m_lightPos);
    // specular
    m_program->SetUniform("viewPos", m_cameraPos);
    m_program->SetUniform("specularStrength", m_specularStrength);
    m_program->SetUniform("specularShininess", m_specularShininess);



    for (size_t i = 0; i < cubePositions.size(); i++){
        auto& pos = cubePositions[i];
        auto model = glm::translate(glm::mat4(1.0f), pos);
        model = glm::rotate(model,
            glm::radians((m_animation ? (float)glfwGetTime() : 0.0f) * 120.0f + 20.0f * (float)i),
            glm::vec3(1.0f, 0.5f, 0.0f));
        auto transform = projection * view * model;
        m_program->SetUniform("transform", transform);
        // model transform uniform 설정
        m_program->SetUniform("modelTransform", model);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }
}

void Context::ProcessInput(GLFWwindow *window) {
  if (!m_cameraControl) return;

  const float cameraSpeed = 0.05f;
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    m_cameraPos += cameraSpeed * m_cameraFront;
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    m_cameraPos -= cameraSpeed * m_cameraFront;
  
  auto cameraRight = glm::normalize(glm::cross(m_cameraUp, -m_cameraFront));
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    m_cameraPos += cameraSpeed * cameraRight;
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    m_cameraPos -= cameraSpeed * cameraRight;

  auto cameraUp = glm::normalize(glm::cross(-m_cameraFront, cameraRight));
  if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    m_cameraPos += cameraSpeed * cameraUp;
  if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    m_cameraPos -= cameraSpeed * cameraUp;
}


void Context::Reshape(int width, int height) {
  m_width = width;
  m_height = height;
  glViewport(0, 0, width, height);
}

void Context::MoveMouse(double x, double y) {
  if (!m_cameraControl) return;

  static glm::vec2 prevPos = glm::vec2((float)x, (float)y);

  auto pos = glm::vec2((float)x, (float)y);
  auto deltaPos = pos - m_prevMousePos;

  const float cameraRotSpeed = 0.08f;
  m_cameraYaw -= deltaPos.x * cameraRotSpeed;
  m_cameraPitch -= deltaPos.y * cameraRotSpeed;

  if (m_cameraYaw < 0.0f) m_cameraYaw += 360.0f;
  if (m_cameraYaw > 360.0f) m_cameraYaw -= 360.0f;

  if (m_cameraPitch < -89.0f) m_cameraPitch = -89.0f;
  if (m_cameraPitch > 89.0f) m_cameraPitch = 89.0f;

  m_prevMousePos = pos;
}

void Context::MouseButton(int button, int action, double x, double y) {
  if (button == GLFW_MOUSE_BUTTON_RIGHT) {
    if (action == GLFW_PRESS) {
      m_prevMousePos = glm::vec2((float)x, (float)y);
      m_cameraControl = true;
    } else if (action == GLFW_RELEASE) {
      m_cameraControl = false;
    }
  }
}