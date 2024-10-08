#ifndef __PROGRAM_H__
#define __PROGRAM_H__

#include "common.h"
#include "shader.h"

CLASS_PTR(Program)
class Program {
  public:
    static ProgramUPtr Create(const std::vector<ShaderPtr>& shaders);
    // add variable
    static ProgramUPtr Create(const std::string& vertex_shader, const std::string& fragment_shader);

    ~Program();
    uint32_t Get() const { return m_program; }
    void Use() const;

    void SetUniform(const std::string& name, int value) const;
    void SetUniform(const std::string& name, const glm::mat4& value) const;

    // add variable
    void SetUniform(const std::string& name, float value) const;
    void SetUniform(const std::string& name, const glm::vec3& value) const;

    void SetUniform(const std::string& name, const glm::vec4& value) const;

  private:
    Program() {};
    bool Link(const std::vector<ShaderPtr>& shaders);
    uint32_t m_program { 0 };
};

#endif
