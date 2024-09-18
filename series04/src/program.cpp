#include "program.h"

ProgramUPtr Program::Create(const std::vector<ShaderPtr>& shaders) {
  auto program = ProgramUPtr(new Program());
  if (!program->Link(shaders)) {
    return nullptr;
  }
  return std::move(program);
};

Program::Program() {
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

