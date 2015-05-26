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

#include "howardgl.hxx"
#include "SceneManager.hxx"

namespace Howard {

namespace Verdandi {

constexpr const char TextureImage::m_asset_type[];
constexpr const char Texture::m_asset_type[];

TextureImage::TextureImage(const std::string& name) : Asset(name.c_str()), size(0, 0) {
    VGLIDX texture_t;
    glGenTextures(1, &texture_t);
    this->m_texid = texture_t;
}

void TextureImage::load(const Image& image) {
    assert(image.ok());
    this->size = image.size;
    glBindTexture(GL_TEXTURE_2D, m_texid);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.ptr());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
}

}

}
