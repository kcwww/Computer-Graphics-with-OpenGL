#include "shadow_map.h"

ShadowMapUPtr ShadowMap::Create(int width, int height)
{
  auto shadowMap = ShadowMapUPtr(new ShadowMap());
  if (!shadowMap->Init(width, height))
    return nullptr;
  return std::move(shadowMap);
}

ShadowMap::~ShadowMap()
{
  if (m_framebuffer)
  {
    glDeleteFramebuffers(1, &m_framebuffer);
  }
}

void ShadowMap::Bind() const
{
  glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
}

bool ShadowMap::Init(int width, int height)
{
  glGenFramebuffers(1, &m_framebuffer); // 프레임버퍼 생성
  Bind();

  m_shadowMap = Texture::Create(width, height, GL_DEPTH_COMPONENT, GL_FLOAT); // 깊이 텍스처 생성, depth component 사용
  m_shadowMap->SetFilter(GL_NEAREST, GL_NEAREST);                             // 필터링 설정
  m_shadowMap->SetWrap(GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);               // 래핑 설정

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                         GL_TEXTURE_2D, m_shadowMap->Get(), 0); // 깊이 텍스처를 프레임버퍼에 바인딩
  glDrawBuffer(GL_NONE);                                        // 깊이 텍스처에 렌더링하기 위해 그리기 버퍼를 비활성화
  glReadBuffer(GL_NONE);                                        // 읽기 버퍼를 비활성화
  auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);       // 프레임버퍼 상태 확인
  if (status != GL_FRAMEBUFFER_COMPLETE)
  {
    SPDLOG_ERROR("failed to complete shadow map framebuffer: {:x}", status);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return false;
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0); // 프레임버퍼 바인딩 해제
  return true;
}
