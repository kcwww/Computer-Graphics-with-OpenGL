#include "program.h"

ProgramUPtr Program::Create(const std::vector<ShaderPtr>& shaders) {
  auto program = ProgramUPtr(new Program());
  if (!program->Link(shaders)) {
    return nullptr;
  }
  return std::move(program);
};

ProgramUPtr Program::Create(const std::string& vertex_shader, const std::string& fragment_shader) {
  ShaderPtr vs = Shader::CreateFromFile(vertex_shader, GL_VERTEX_SHADER);
  ShaderPtr fs = Shader::CreateFromFile(fragment_shader, GL_FRAGMENT_SHADER);
  if (!vs || !fs) {
    return nullptr;
  }
  return std::move(Create({vs, fs}));
};

Program::~Program() {
  if (m_program) {
    glDeleteProgram(m_program);
  }
};

bool Program::Link(const std::vector<ShaderPtr>& shaders) {
  m_program = glCreateProgram(); // create a program object
  for (auto& shader : shaders) {
    glAttachShader(m_program, shader->Get()); // attach shader to program
  }
  glLinkProgram(m_program); // link program

  int success = 0;
  glGetProgramiv(m_program, GL_LINK_STATUS, &success); // check if linking is successful
  if (!success) {
    char infoLog[1024];
    glGetProgramInfoLog(m_program, 1024, nullptr, infoLog); // get error log
    SPDLOG_ERROR("failed to link program: {}", infoLog);
    return false;
  }

  return true;
};

void Program::Use() const {
  glUseProgram(m_program); // use program
};


void Program::SetUniform(const std::string& name, int value) const {
  auto loc = glGetUniformLocation(m_program, name.c_str()); // get uniform location
  glUniform1i(loc, value); // set uniform  
};

void Program::SetUniform(const std::string& name, const glm::mat4& value) const {
  auto loc = glGetUniformLocation(m_program, name.c_str()); // get uniform location
  glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value)); // set uniform
};

void Program::SetUniform(const std::string& name, float value) const {
  auto loc = glGetUniformLocation(m_program, name.c_str()); // get uniform location
  glUniform1f(loc, value); // set uniform
};

void Program::SetUniform(const std::string& name, const glm::vec2& value) const {
  auto loc = glGetUniformLocation(m_program, name.c_str()); // get uniform location
  glUniform2fv(loc, 1, glm::value_ptr(value)); // 1 은 벡터의 개수
};

void Program::SetUniform(const std::string& name, const glm::vec3& value) const {
  auto loc = glGetUniformLocation(m_program, name.c_str()); // get uniform location
  glUniform3fv(loc, 1, glm::value_ptr(value)); // 1 은 벡터의 개수
};

void Program::SetUniform(const std::string& name, const glm::vec4& value) const {
  auto loc = glGetUniformLocation(m_program, name.c_str()); // get uniform location
  glUniform4fv(loc, 1, glm::value_ptr(value)); // 1 은 벡터의 개수
};
