#include "framebuffer.h"

FramebufferUPtr Framebuffer::Create(const TexturePtr colorAttachment)
{
  auto framebuffer = FramebufferUPtr(new Framebuffer());
  if (!framebuffer->InitWithColorAttachment(colorAttachment)) // colorAttachment을 이용하여 초기화
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

bool Framebuffer::InitWithColorAttachment(const TexturePtr colorAttachment)
{
  // color buffer 생성
  m_colorAttachment = colorAttachment;
  glGenFramebuffers(1, &m_framebuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

  // framebuffer에 color attachment을 붙임
  glFramebufferTexture2D(GL_FRAMEBUFFER,
                         GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         colorAttachment->Get(), 0);

  // depth, stencil buffer 생성 및 framebuffer에 붙임
  glGenRenderbuffers(1, &m_depthStencilBuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, m_depthStencilBuffer);
  glRenderbufferStorage(
      GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
      colorAttachment->GetWidth(), colorAttachment->GetHeight());
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
