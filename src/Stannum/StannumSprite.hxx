//
// Made by secondwtq <lovejay-lovemusic@outlook.com> with Love.
//
// Date: 2015-05-29
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of the HOWARD11 Game Engine.
//
// WE ARE STANDING ON THE EDGE.
//

#ifndef HOWARD11_STANNUMSPRITE_HXX
#define HOWARD11_STANNUMSPRITE_HXX

#include "Stannum.hxx"
#include "StannumTexture.hxx"

#include <glm/glm.hpp>

namespace Howard {
namespace Stannum {

struct DataSprite {

    VertFormatSprite data[6];
    const Verdandi::Texture *texture;

    DataSprite() {
        set_color({ 1.0, 1.0, 1.0, 1.0 }); }

    void set_texture_and_pos(const Verdandi::Texture *texture, const glm::vec2& pos) {
        assert(texture != nullptr); // 150623: NO `this`! We just wanna check the arg, fixed.
        glm::vec2 size = { texture->size };
        this->set_position_and_size(pos, size);
        this->texture = texture;

        data[0].texcoord = data[5].texcoord = glm::vec3(texture->pos_start, 0);
        data[4].texcoord = glm::vec3(texture->pos_end.x, texture->pos_start.y, 0);
        data[2].texcoord = data[3].texcoord = glm::vec3(texture->pos_end, 0);
        data[1].texcoord = glm::vec3(texture->pos_start.x, texture->pos_end.y, 0);
    }

    void set_position(const glm::vec2& pos) {
        assert(this->texture != nullptr);
        glm::vec2 size = { texture->size };
        this->set_position_and_size(pos, size);
    }

    void set_position_and_size(const glm::vec2& location, const glm::vec2& size) {
        glm::vec2 size_ = size / (float) 2.0;
        data[0].position = data[5].position = glm::vec3(location.x - size_.x,
                location.y - size_.y, 0);
        data[4].position = glm::vec3(location.x + size_.x,
                location.y - size_.y, 0);
        data[2].position = data[3].position = glm::vec3(location.x + size_.x,
                location.y + size_.y, 0);
        data[1].position = glm::vec3(location.x - size_.x,
                location.y + size_.y, 0);
    }

    void set_color(const glm::vec4& color) {
        for (size_t i = 0; i < 6; i++) {
            data[i].multiply = color; }
    }

};

class CommandSprite : public StannumCommand {
    friend class StannumRenderer;
public:
    CommandSprite(DataSprite *data)
            : m_data(data) { }
    CommandType cmd_type() const override {
        return CommandType::CTest; }
    virtual void execute(StannumRenderer *renderer) override;
private:
    DataSprite *m_data = nullptr;
public:
    static MappedVertexBuffer<VertFormatSprite> *m_buf;
};

class DispatchCommandSprite : public StannumDispatchCommand {
public:
    DispatchCommandSprite() { }
    DispatchCommandType commandType() const override {
        return DispatchCommandType::DSpriteDispatch; }
    virtual void execute(StannumRenderer *renderer) override;
};

}
}

#endif // HOWARD11_STANNUMSPRITE_HXX
