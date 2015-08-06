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

#include <string.h>
#include <cstdint>
#include <vector>
#include <string>

#include "Common.hxx"
#include "Debug.hxx"
#include <glm/glm.hpp>
#include <stb/stb_image.h>
#include <utility>

#include "FSM/FSMHelper.hxx"
#include "Asset.hxx"
#include "Verdandi/GLCommon.hxx"

#define HO_SIZE_DEFAULT (glm::u16vec2(-1, -1))
#define HO_UPOS_DEFAULT (glm::u16vec2(-1, -1))

namespace Howard {

namespace Verdandi {

using namespace FSMHelper;

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

    void load_from_mem(const RawDataT *data, size_t len) {
        assert(data); assert(len);
        assert(m_image.empty());

        int width, height, channels;
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
    };

    const RawDataT *ptr() const { return m_image.data(); }

    private:

    std::string m_name;
    std::vector<RawDataT> m_image;

};

class TextureImage : public Asset {

    public:

    TextureImage(const std::string& name);

    const char *asset_type() const override { return TextureImage::m_asset_type; }
    static constexpr const char m_asset_type[] = "TextureImage";

    glm::u16vec2 size;

    void load(const Image& image);

    VGLIDX id() const { return m_texid; }

    private:

    VGLIDX m_texid;

};

class Texture : public Asset {

    public:

    Texture(const std::string& name, const TextureImage *parent) :
            Texture(name, parent, HO_UPOS_DEFAULT, HO_SIZE_DEFAULT) { }

    Texture(const std::string& name, const TextureImage *parent, const glm::u16vec2& pp,
            const glm::u16vec2& ss) : Asset(name), pos(pp), size(ss), m_parent(parent) {
        if (pp == HO_UPOS_DEFAULT)
            pos = { 0, 0 };
        if (size == HO_SIZE_DEFAULT)
            size = parent->size;

        glm::vec2 pos_ = pos, size_ = size, psize = parent->size;
        pos_start = pos_ / psize;
        pos_end = (pos_ + size_) / psize;
    }

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

}

}

#endif // HOWARD11_STANNUMTEXTURE_HXX
