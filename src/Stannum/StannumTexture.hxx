//
// Made by secondwtq <lovejay-lovemusic@outlook.com> with Love.
//
// Date: 2015-05-24
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of the HOWARD11 Game Engine.
//
// WE ARE STANDING ON THE EDGE.
//

#ifndef HOWARD11_STANNUMTEXTURE_HXX
#define HOWARD11_STANNUMTEXTURE_HXX

#include <stdint.h>
#include <vector>
#include <string>

#include "Common.hxx"
#include "Debug.hxx"
#include <glm/glm.hpp>

#include "Asset.hxx"
#include "Verdandi/GLCommon.hxx"

#define HO_SIZE_DEFAULT (glm::u16vec2(-1, -1))
#define HO_UPOS_DEFAULT (glm::u16vec2(-1, -1))

namespace Howard {
namespace Verdandi {

enum ImageChannelType {
    INONE,
    IRGBA,
    IRGB,
    IGRAY,
    IGRAYA
};

enum ImageSourceFormat {
    FNone,
    FPNG
};

class ImageHelper {
public:
    inline static size_t numberOfChannels(ImageChannelType channel) {
        switch (channel) {
            case IRGBA:
                return 4;
            case IRGB:
                return 3;
            case IGRAY:
                return 1;
            case IGRAYA:
                return 2;
            default:
                return 0;
        }
    }

    static unsigned int stbTypeForChannel(ImageChannelType channel);
    static VGLIDX glEnumForChannelFormat(ImageChannelType channel);
};

class TextureImage;

class Image {

    public:

    Image(const char *name) : size(0, 0), m_name(name) { }

    Image(const Image&) = delete;
    Image& operator = (const Image&) = delete;

    Image(Image&& other) : size(other.size),
        m_name(std::move(other.m_name)), m_image(std::move(other.m_image)) { }

    Image& operator = (Image&& other) {
        this->m_name = std::move(other.m_name);
        this->m_image = std::move(other.m_image);
        this->size = other.size;
        return *this;
    }

    glm::u16vec2 size;

    bool ok() const { return !(m_image.empty()); }

    void loadFromImage(const Image& other);
    void loadFromImageAndResize(const Image& other, const HPixel& size);
    void loadFromMemory(const RawDataT *data, size_t len, ImageChannelType channel_fmt,
                ImageSourceFormat format = ImageSourceFormat::FPNG);
    void load_from_mem(const RawDataT *data, size_t len);
    void loadFromTextureImage(const TextureImage& other);

    void resize(const HPixel& size);
    void scale(const HScale& val) {
        ASSERT(val.x > 0 && val.y > 0);
        this->resize({ size.x * val.x, size.y * val.y }); }

    const RawDataT *ptr() const { return m_image.data(); }

    ImageChannelType channelFormat() const {
        return m_channel_type; }

    inline void reset() {
        m_channel_type = INONE;
        size = { 0, 0 };
        m_image.clear();
    }

private:

    ImageChannelType m_channel_type = INONE;
    std::string m_name;
    std::vector<RawDataT> m_image;
};

class TextureImage : public Asset {
public:

    TextureImage(const std::string& name);
    ~TextureImage();

    const char *asset_type() const override { return TextureImage::m_asset_type; }
    static constexpr const char m_asset_type[] = "TextureImage";

    HPixel size;

    void loadFromImage(const Image& image);
    void loadFromTextureImage(const TextureImage& image);
    void loadEmpty(const HPixel& size, ImageChannelType channelfmt);

    VGLIDX id() const { return m_texid; }

    ImageChannelType channelFormat() const {
        return m_channel_type; }

    inline bool ok() const {
        return m_channel_type != INONE; }

private:

    void cleanupForReload();

    ImageChannelType m_channel_type = INONE;
    VGLIDX m_texid;
};

class Texture : public Asset {
public:

    Texture(const std::string& name, const TextureImage *parent) :
            Texture(name, parent, HO_UPOS_DEFAULT, HO_SIZE_DEFAULT) { }

    Texture(const std::string& name, const TextureImage *parent, const HPixel& pp,
            const HPixel& ss) : Asset(name), pos(pp), size(ss), m_parent(parent) {
        if (pp == HO_UPOS_DEFAULT)
            pos = { 0, 0 };
        if (size == HO_SIZE_DEFAULT)
            size = parent->size;

        glm::vec2 pos_ = pos, size_ = size, psize = parent->size;
        pos_start = pos_ / psize;
        pos_end = (pos_ + size_) / psize;
    }

    static Texture *createWithEntireImage(const std::string& name, const TextureImage *parent) {
        return new Texture(name, parent); }

    static Texture *createWithPartialImage(const std::string& name,
            const TextureImage *parent, const HPixel& position, const HPixel& size) {
        return new Texture(name, parent, position, size); }

    VGLIDX id() const { return m_parent->id(); }

    const char *asset_type() const override { return Texture::m_asset_type; }
    static constexpr const char m_asset_type[] = "Texture";

    glm::u16vec2 pos;
    glm::u16vec2 size;

    glm::vec2 pos_start;
    glm::vec2 pos_end;

private:

    const TextureImage *m_parent;
};

class TextureAtlas : public Asset {
public:
    TextureAtlas(const std::string& name);

private:


};

}

}

#endif // HOWARD11_STANNUMTEXTURE_HXX
