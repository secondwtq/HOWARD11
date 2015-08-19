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

#ifndef HOWARD11_STANNUMTESTNODE_HXX
#define HOWARD11_STANNUMTESTNODE_HXX

#include <glm/glm.hpp>

#include "Node.hxx"
#include "Stannum/StannumSprite.hxx"
#include "Stannum/Stannum.hxx"

#include "Misc/Transform.hxx"

namespace Howard {

// 150530: SpriteNode works.
class StannumSpriteNode : public HNode {

public:

    StannumSpriteNode (class RootNode *scene, Verdandi::Texture *texture)
            : HNode(scene), camera(Transform::IsometricCamera::instance) {
        m_data.set_texture_and_pos(texture, { 0, 0 }); }

    static StannumSpriteNode *create(RootNode *scene, Verdandi::Texture *texture) {
        return new StannumSpriteNode(scene, texture); }

    virtual HowardNodeType node_typeid() const { return HowardNodeType::NStannumSpriteNode; }
    virtual const char *node_type() const { return StannumSpriteNode::m_node_type; }

    HCoord position() const {
        return m_position; }

    void set_position(const HCoord& pos) {
        this->m_position = pos; }

    void onPaint(Stannum::RenderQueue *queue) override {
        m_data.set_position({ Transform::view_pos(m_position, camera) });
        queue->push(new Stannum::CommandSprite(&m_data)); }

    static constexpr const char m_node_type[] = "StannumSpriteNode";

    Transform::Camera *camera = nullptr;

    private:

    HCoord m_position;
    Stannum::DataSprite m_data;

};

}

#endif // HOWARD11_STANNUMTESTNODE_HXX
