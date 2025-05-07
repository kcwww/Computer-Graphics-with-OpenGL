#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "common.h"

CLASS_PTR(Image)
class Image
{
public:
    static ImageUPtr Load(const std::string &filepath, bool filpVertical = true);
    static ImageUPtr Create(int width, int height,
                            int channelCount = 4, int bytePerChannel = 1); // 채널 1개당 바이트 수
    static ImageUPtr CreateSingleColorImage(int width, int height, const glm::vec4 &color);
    ~Image();

    const uint8_t *GetData() const { return m_data; }
    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }
    int GetChannelCount() const { return m_channelCount; }
    int GetBytePerChannel() const { return m_bytePerChannel; } // 채널 1개당 바이트 수

    void SetCheckImage(int gridX, int gridY);

private:
    Image() {};
    bool LoadWithStb(const std::string &filepath, bool flipVertical);
    bool Allocate(int width, int height, int channelCount, int bytePerChannel); // bytePerChannel 추가
    int m_width{0};
    int m_height{0};
    int m_channelCount{0};
    int m_bytePerChannel{1}; // 채널 1개당 바이트 수
    uint8_t *m_data{nullptr};
};

#endif // __IMAGE_H__
