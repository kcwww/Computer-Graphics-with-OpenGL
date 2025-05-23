#include "framebuffer.h"

FramebufferUPtr Framebuffer::Create(const std::vector<TexturePtr> &colorAttachments)
{
  auto framebuffer = FramebufferUPtr(new Framebuffer());
  if (!framebuffer->InitWithColorAttachments(colorAttachments)) // colorAttachment을 이용하여 초기화
    return nullptr;
  return std::move(framebuffer);
}

void Framebuffer::BindToDefault() // 디폴트 화면으로 바인딩
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Framebuffer::~Framebuffer()
{
  if (m_depthStencilBuffer)
  {
    glDeleteRenderbuffers(1, &m_depthStencilBuffer);
  }
  if (m_framebuffer)
  {
    glDeleteFramebuffers(1, &m_framebuffer);
  }
}

void Framebuffer::Bind() const // framebuffer로 바인딩
{
  glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
}

bool Framebuffer::InitWithColorAttachments(const std::vector<TexturePtr> &colorAttachments)
{
  // color buffer 생성
  m_colorAttachments = colorAttachments;
  glGenFramebuffers(1, &m_framebuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

  // framebuffer에 color attachments을 붙임
  for (size_t i = 0; i < m_colorAttachments.size(); i++)
  {
    glFramebufferTexture2D(
        GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i,
        GL_TEXTURE_2D, m_colorAttachments[i]->Get(), 0);
  }
  if (m_colorAttachments.size() > 0)
  {
    std::vector<GLenum> attachments;
    attachments.resize(m_colorAttachments.size());
    for (size_t i = 0; i < m_colorAttachments.size(); i++)
      attachments[i] = GL_COLOR_ATTACHMENT0 + i;

    glDrawBuffers(m_colorAttachments.size(), attachments.data());
  }
  int width = m_colorAttachments[0]->GetWidth();
  int height = m_colorAttachments[0]->GetHeight();

  // depth, stencil buffer 생성 및 framebuffer에 붙임
  glGenRenderbuffers(1, &m_depthStencilBuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, m_depthStencilBuffer);
  glRenderbufferStorage(
      GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
      width, height); // depth buffer와 stencil buffer를 동시에 생성
  glBindRenderbuffer(GL_RENDERBUFFER, 0);

  glFramebufferRenderbuffer(
      GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
      GL_RENDERBUFFER, m_depthStencilBuffer);

  // framebuffer이 제대로 생성되었는지 확인
  auto result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (result != GL_FRAMEBUFFER_COMPLETE)
  {
    SPDLOG_ERROR("failed to create framebuffer: {}", result);
    return false;
  }

  // framebuffer로 바인딩 해제
  BindToDefault();

  return true;
}
