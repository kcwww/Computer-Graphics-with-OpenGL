#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "common.h"
#include "shader.h"
#include "program.h"
#include "buffer.h"
#include "vertex_layout.h"
#include "texture.h"

CLASS_PTR(Context)
class Context
{
public:
  static ContextUPtr Create();
  void Render();

  // input
  void ProcessInput(GLFWwindow *window);

  // WIDTH, HEIGHT
  void Reshape(int width, int height);

  // move mouse
  void MoveMouse(double x, double y);
  void MouseButton(int button, int action, double x, double y);

private:
  Context() {};
  bool Init();
  ProgramUPtr m_program;
  ProgramUPtr m_simpleProgram;

  VertexLayoutUPtr m_vertexLayout;
  BufferUPtr m_vertexBuffer;
  BufferUPtr m_elementBuffer;
  TextureUPtr m_texture;
  TextureUPtr m_texture2;

  // camera parameter
  bool m_cameraControl{false};
  glm::vec2 m_prevMousePos{glm::vec2(0.0f)};
  glm::vec3 m_cameraPos{0.0f, 0.0f, 3.0f};
  glm::vec3 m_cameraFront{0.0f, 0.0f, -1.0f};
  glm::vec3 m_cameraUp{0.0f, 1.0f, 0.0f};

  // mouse
  float m_cameraPitch{0.0f};
  float m_cameraYaw{0.0f};

  // size
  int m_width{WINDOW_WIDTH};
  int m_height{WINDOW_HEIGHT};

  glm::vec4 m_clearColor{glm::vec4(0.1f, 0.2f, 0.3f, 0.0f)};

  bool m_animation{true};

  struct Light
  {
    // direction light
    // glm::vec3 direction{glm::vec3(-0.2f, -1.0f, -0.3f)}; // light direction
    
    // point light
    glm::vec3 position{glm::vec3(2.0f, 2.0f, 2.0f)};
    float distance { 32.0f };

    // spot light
    glm::vec3 direction {glm::vec3(-1.0f, -1.0f, -1.0f)};
    float cutOff { 20.0f };

    glm::vec3 ambient{glm::vec3(0.1f)};
    glm::vec3 diffuse{glm::vec3(0.5f)};
    glm::vec3 specular{glm::vec3(1.0f)};
  };
  Light m_light;

  struct Material
  {
    TextureUPtr diffuse;
    TextureUPtr specular;
    float shininess{32.0f};
  };
  Material m_material;
};

#endif
