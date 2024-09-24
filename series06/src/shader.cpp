#include "shader.h"

ShaderUPtr Shader::CreateFromFile(const std::string& filename, GLenum shaderType) {
    ShaderUPtr shader = std::unique_ptr<Shader>(new Shader());
    if (!shader->LoadFile(filename,
                          shaderType)) return nullptr;

    return std::move(shader);
}

Shader::Shader() {};

Shader::~Shader() {
    if (m_shader) {
        glDeleteShader(m_shader); // delete shader
    }
}

bool Shader::LoadFile(const std::string& filename, GLenum shaderType) {
    auto text = LoadTextFile(filename);
    if (!text.has_value()) return false; // failed to load file

    auto &code = text.value();
    const char *codePtr = code.c_str();
    int32_t codeLength = (int32_t)code.length();

    // create shader
    m_shader = glCreateShader(shaderType);
    glShaderSource(m_shader, 1, (const GLchar * const *)&codePtr, &codeLength); // set shader source
    glCompileShader(m_shader);

    // check for compilation errors
    int success = 0;
    glGetShaderiv(m_shader, GL_COMPILE_STATUS, &success); // get compile status
    if (!success) {
      char infolog[1024];
      glGetShaderInfoLog(m_shader, 1024, nullptr, infolog);
      SPDLOG_ERROR("Failed to compile shader: \"{}\"", filename);
      SPDLOG_ERROR("reason : {}", infolog);
      return false;
    }
  return true;
}
