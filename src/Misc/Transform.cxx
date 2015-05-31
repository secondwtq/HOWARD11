//
// Made by secondwtq <lovejay-lovemusic@outlook.com> with Love.
//
// Date: 2015-05-27
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of the HOWARD11 Game Engine.
//
// WE ARE STANDING ON THE EDGE.
//

#include "Transform.hxx"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Common.hxx"

namespace Howard {

namespace Transform {

IsometricCamera *IsometricCamera::instance = new IsometricCamera;

void IsometricCamera::update() {
    this->view_mat = glm::lookAt(this->pos, -this->look_at, this->vec_up); }

HPoint view_pos(const glm::i32vec3& coord, const Camera *camera) {
    glm::vec4 ret { -coord, 1 };
    ret = camera->view_mat * ret * camera->scale_factor;
    return HPoint(ret);
}

}

}