#ifndef __FRAMEBUFFER_H__
#define __FRAMEBUFFER_H__

#include "texture.h"

CLASS_PTR(Framebuffer);
class Framebuffer
{
public:
  static FramebufferUPtr Create(const std::vector<TexturePtr> &colorAttachments); // 여러개의 color attachment을 받는 생성자
  static void BindToDefault();
  ~Framebuffer();

  const uint32_t Get() const { return m_framebuffer; }
  void Bind() const;
  int GetColorAttachmentCount() const { return static_cast<int>(m_colorAttachments.size()); }
  const TexturePtr GetColorAttachment(int index = 0) const { return m_colorAttachments[index]; }

private:
  Framebuffer() {}
  bool InitWithColorAttachments(const std::vector<TexturePtr> &colorAttachments);

  uint32_t m_framebuffer{0};
  uint32_t m_depthStencilBuffer{0};
  std::vector<TexturePtr> m_colorAttachments;
};

void CubeTexture::Init(int width, int height, int format, uint32_t type)
{
  glGenTextures(1, &m_texture);
  Bind();

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  m_width = width;
  m_height = height;
  m_type = type;
  m_format = format;
  GLenum imageFormat = GetImageFormat(m_format);

  for (uint32_t i = 0; i < 6; i++)
  {
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, m_format, m_width, m_height, 0, imageFormat, m_type, nullptr);
  }
}

#endif // __FRAMEBUFFER_H__
