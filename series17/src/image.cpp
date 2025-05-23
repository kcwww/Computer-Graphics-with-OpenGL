#include "image.h"
#include <stb/stb_image.h>

ImageUPtr Image::Load(const std::string &filepath, bool flipVertical)
{
    auto image = ImageUPtr(new Image());
    if (!image->LoadWithStb(filepath, flipVertical))
        return nullptr;
    return std::move(image);
}

ImageUPtr Image::Create(int width, int height, int channelCount, int bytePerChannel) // bytePerChannel 추가
{
    auto image = ImageUPtr(new Image());
    if (!image->Allocate(width, height, channelCount, bytePerChannel))
        return nullptr;
    return std::move(image);
}

ImageUPtr Image::CreateSingleColorImage(int width, int height, const glm::vec4 &color)
{
    glm::vec4 clamped = glm::clamp(color * 255.0f, 0.0f, 255.0f);
    uint8_t rgba[4] = {
        (uint8_t)clamped.r,
        (uint8_t)clamped.g,
        (uint8_t)clamped.b,
        (uint8_t)clamped.a,
    };
    auto image = Create(width, height, 4);
    for (int i = 0; i < width * height; i++)
    {
        memcpy(image->m_data + 4 * i, rgba, 4);
    }
    return std::move(image);
}

Image::~Image()
{
    if (m_data)
    {
        stbi_image_free(m_data);
    }
}

void Image::SetCheckImage(int gridX, int gridY)
{
    for (int j = 0; j < m_height; j++)
    {
        for (int i = 0; i < m_width; i++)
        {
            int pos = (j * m_width + i) * m_channelCount;
            bool even = ((i / gridX) + (j / gridY)) % 2 == 0;
            uint8_t value = even ? 255 : 0;
            for (int k = 0; k < m_channelCount; k++)
                m_data[pos + k] = value;
            if (m_channelCount > 3)
                m_data[3] = 255;
        }
    }
}

bool Image::LoadWithStb(const std::string &filepath, bool flipVertical) // hdr 확장자 로드
{
    stbi_set_flip_vertically_on_load(flipVertical);
    auto ext = filepath.substr(filepath.find_last_of('.'));
    if (ext == ".hdr" || ext == ".HDR")
    {
        m_data = (uint8_t *)stbi_loadf(filepath.c_str(), &m_width, &m_height, &m_channelCount, 0); // float 타입 로드
        m_bytePerChannel = 4;
    }
    else
    {
        m_data = stbi_load(filepath.c_str(), &m_width, &m_height, &m_channelCount, 0); // uint8_t 타입 로드
        m_bytePerChannel = 1;
    }
    if (!m_data)
    {
        SPDLOG_ERROR("failed to load image: {}", filepath);
        return false;
    }
    return true;
}

bool Image::Allocate(int width, int height, int channelCount, int bytePerChannel) // bytePerChannel 추가
{
    m_width = width;
    m_height = height;
    m_channelCount = channelCount;
    m_bytePerChannel = bytePerChannel;
    m_data = (uint8_t *)malloc(m_width * m_height * m_channelCount * m_bytePerChannel);
    return m_data ? true : false;
}
