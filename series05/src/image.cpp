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
  stbi_set_flip_vertically_on_load(true); // 이미지를 로드할 때 y축을 뒤집습니다.
  m_data = stbi_load(filepath.c_str(), &m_width, &m_height, &m_channelCount, 0); // stbi 로드 함수를 이용해 이미지를 로드합니다.
  if (!m_data) {
    SPDLOG_ERROR("Failed to load image: {}", filepath);
    return false;
  }
  return true;
}

ImageUPtr Image::Create(int width, int height, int channelCount) {
  auto image = ImageUPtr(new Image());
  if (!image->Allocate(width, height, channelCount)) {
    return nullptr;
  }
  return std::move(image);
}

bool Image::Allocate(int width, int height, int channelCount) {
  m_width = width;
  m_height = height;
  m_channelCount = channelCount;
  m_data = (uint8_t*)malloc(m_width * m_height * m_channelCount); // 이미지 데이터를 저장할 메모리를 할당합니다.
  return m_data ? true : false;
}

// SetCheckImage 함수는 체크 이미지를 만드는 함수입니다.
void Image::SetCheckImage(int gridX, int gridY) {
  for (int j = 0; j < m_height; j++) {
    for (int i = 0; i< m_width; i++) {
      int pos = j * m_width * m_channelCount + i * m_channelCount;
      bool even = ((i / gridX) + (j / gridY)) % 2 == 0;
      uint8_t value = even ? 255 : 0;
      for (int k = 0; k < m_channelCount; k++) {
        m_data[pos + k] = value;
        if (m_channelCount > 3) m_data[3] = 255;
      }
    }
  }
}
