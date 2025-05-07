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

CLASS_PTR(CubeFramebuffer);
class CubeFramebuffer
{
public:
  static CubeFramebufferUPtr Create(const CubeTexturePtr colorAttachment, uint32_t mipLevel = 0); // mip level을 받는 생성자
  ~CubeFramebuffer();

  const uint32_t Get() const { return m_framebuffer; }
  void Bind(int cubeIndex = 0) const;
  const CubeTexturePtr GetColorAttachment() const { return m_colorAttachment; }

private:
  CubeFramebuffer() {}
  bool InitWithColorAttachment(const CubeTexturePtr &colorAttachment, uint32_t mipLevel); // mip level을 받아 초기화

  uint32_t m_framebuffer{0};
  uint32_t m_depthStencilBuffer{0};
  CubeTexturePtr m_colorAttachment;
  // mip level
  uint32_t m_mipLevel{0};
};

#endif // __FRAMEBUFFER_H__
