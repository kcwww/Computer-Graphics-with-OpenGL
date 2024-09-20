#include "image.h"
#define STB_IMAGE_IMPLEMENTATION // stb 라이브러리를 사용하기 위해 해당 매크로를 정의합니다.
#include <stb/stb_image.h>

ImageUPtr Image::Load(const std::string& filepath) {
  auto image = ImageUPtr(new Image());
  if (!image->LoadWithStb(filepath)) {
    return nullptr;
  }
  return std::move(image);
}

Image::~Image() {
  if (m_data) {
    stbi_image_free(m_data);
  }
}

bool Image::LoadWithStb(const std::string& filepath) {
  m_data = stbi_load(filepath.c_str(), &m_width, &m_height, &m_channelCount, 0); // stbi 로드 함수를 이용해 이미지를 로드합니다.
  if (!m_data) {
    SPDLOG_ERROR("Failed to load image: {}", filepath);
    return false;
  }
  return true;
}
