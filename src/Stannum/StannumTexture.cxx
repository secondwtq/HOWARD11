//
// Made by secondwtq <lovejay-lovemusic@outlook.com> with Love.
//
// Date: 2015-05-26
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of the HOWARD11 Game Engine.
//
// WE ARE STANDING ON THE EDGE.
//

#include "StannumTexture.hxx"

#include "FSM/FSMHelper.hxx"

#include "thirdpt/stb_image.hxx"
#include "thirdpt/stb_image_resize.hxx"
#include "thirdpt/howardgl.hxx"

#include "JSFunctions.hxx"

#include <string.h>

namespace Howard {
namespace Verdandi {

using namespace FSMHelper;

constexpr const char TextureImage::m_asset_type[];
constexpr const char Texture::m_asset_type[];

void Image::loadFromImage(const Image& other) {
    ASSERT(other.ok());

    this->m_channel_type = other.channelFormat();
    m_image.resize(other.size.x * other.size.y *
                   ImageHelper::numberOfChannels(channelFormat()));
    memcpy(m_image.data(), other.ptr(), m_image.size() * sizeof(RawDataT));
}

void Image::loadFromImageAndResize(const Image& other, const HPixel& size) {
    ASSERT(other.ok());

    this->m_channel_type = other.channelFormat();
    size_t num_channels = ImageHelper::numberOfChannels(channelFormat());
    m_image.resize(size.x * size.y * num_channels);
    ASSERT(stbir_resize_uint8(other.ptr(), other.size.x, other.size.y, 0,
            m_image.data(), size.x, size.y, 0, num_channels));
}

void Image::loadFromMemory(const RawDataT *data, size_t len,
        ImageChannelType channel_fmt, ImageSourceFormat format) {
    ASSERT(data); ASSERT(len);
    //        assert(m_image.empty());

    int width, height, channels;
    RawDataT *decoded = stbi_load_from_memory(data, len,
            &width, &height, &channels, ImageHelper::stbTypeForChannel(channel_fmt));

    if (decoded) {
        size = { width, height };
        m_image.resize(width * height * channels);
        memcpy(m_image.data(), decoded, m_image.size() * sizeof(RawDataT));
        m_channel_type = channel_fmt;
        stbi_image_free(decoded);
    } else {
        log("Verdandi", Warning) << "Failed to load Image " << m_name << ": " <<
        stbi_failure_reason() << rn;
    }
}

void Image::loadFromTextureImage(const TextureImage& other) {
    ASSERT(other.ok());

    size = other.size;
    m_channel_type = other.channelFormat();

    size_t num_channels = ImageHelper::numberOfChannels(channelFormat());
    m_image.resize(size.x * size.y * num_channels);

    VGLIDX fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, other.id(), 0);
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    glReadPixels(0, 0, size.x, size.y,
            ImageHelper::glEnumForChannelFormat(other.channelFormat()),
            GL_UNSIGNED_BYTE, m_image.data());
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteFramebuffers(1, &fbo);
}

void Image::load_from_mem(const RawDataT *data, size_t len) {
    assert(data); assert(len);
    assert(m_image.empty());

    int width, height, channels;
    m_channel_type = IRGBA;
    RawDataT *decoded = stbi_load_from_memory(data, len,
            &width, &height, &channels, STBI_rgb_alpha);

    if (decoded) {
        size = { width, height };
        m_image.resize(width * height * 4);
        memcpy(m_image.data(), decoded, m_image.size() * sizeof(RawDataT));
        stbi_image_free(decoded);
    } else {
        log("Verdandi", Warning) << "Failed to load Image " << m_name << ": " <<
        stbi_failure_reason() << rn;
    }
}

void Image::loadFromFile(const std::string& filename,
        ImageChannelType channel_fmt, ImageSourceFormat format) {
    std::string buf = readfile(filename);
    loadFromMemory(reinterpret_cast<const Howard::RawDataT *>(buf.c_str()),
            buf.length(), channel_fmt, format);
}

void Image::resize(const HPixel& size) {
    std::vector<RawDataT> new_data;
    size_t num_channels = ImageHelper::numberOfChannels(channelFormat());
    new_data.resize(size.x * size.y * num_channels);
    ASSERT(stbir_resize_uint8(ptr(), this->size.x, this->size.y, 0,
            new_data.data(), size.x, size.y, 0, num_channels));
    m_image.swap(new_data);
    this->size = size;
}

TextureImage::TextureImage(const std::string& name) : Asset(name.c_str()), size(0, 0) {
    VGLIDX texture_t;
    glGenTextures(1, &texture_t);
    this->m_texid = texture_t;
}

void TextureImage::loadFromImage(const Image& image) {
    ASSERT(image.ok());

    cleanupForReload();
    this->size = image.size;
    this->m_channel_type = image.channelFormat();
    glBindTexture(GL_TEXTURE_2D, m_texid);
    glTexImage2D(GL_TEXTURE_2D, 0, ImageHelper::glEnumForChannelFormat(m_channel_type),
            size.x, size.y, 0, ImageHelper::glEnumForChannelFormat(m_channel_type),
            GL_UNSIGNED_BYTE, image.ptr());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void TextureImage::loadEmpty(const HPixel& size, ImageChannelType channelfmt) {
//    ASSERT(!ok());

    cleanupForReload();
    this->size = size;
    this->m_channel_type = channelfmt;

    glBindTexture(GL_TEXTURE_2D, m_texid);
    glTexImage2D(GL_TEXTURE_2D, 0, ImageHelper::glEnumForChannelFormat(m_channel_type),
            size.x, size.y, 0, ImageHelper::glEnumForChannelFormat(m_channel_type),
            GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void TextureImage::loadFromTextureImage(const TextureImage& image) {
    ASSERT(image.ok());

    cleanupForReload();
    loadEmpty(image.size, image.channelFormat());

    VGLIDX fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
            GL_TEXTURE_2D, image.id(), 0);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1,
            GL_TEXTURE_2D, id(), 0);
    glDrawBuffer(GL_COLOR_ATTACHMENT1);
    glBlitFramebuffer(0, 0, size.x, size.y, 0, 0, size.x, size.y,
            GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteFramebuffers(1, &fbo);
}

void TextureImage::cleanupForReload() {
    if (m_channel_type != INONE) {
        VGLIDX texture_t = m_texid;
        glDeleteTextures(1, &texture_t);
        glGenTextures(1, &texture_t);
        m_texid = texture_t;
    }
}

TextureImage::~TextureImage() {
    VGLIDX texture_t = m_texid;
    glDeleteTextures(1, &texture_t);
}

VGLIDX ImageHelper::glEnumForChannelFormat(ImageChannelType channel) {
    switch (channel) {
        case IRGBA:
            return GL_RGBA;
        case IRGB:
            return GL_RGB;
        case IGRAY:
            // GL_ALPHA is obsoleted
            return GL_RED;
        default:
            return 0;
    }
}

unsigned int ImageHelper::stbTypeForChannel(ImageChannelType channel) {
    switch (channel) {
        case IRGBA:
            return STBI_rgb_alpha;
        case IRGB:
            return STBI_rgb;
        case IGRAY:
            return STBI_grey;
        case IGRAYA:
            return STBI_grey_alpha;
        default:
            return 0;
    }
}

}
}
