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

#ifndef HOWARD11_TRANSFORM_HXX
#define HOWARD11_TRANSFORM_HXX

#include <glm/glm.hpp>

#include "Common.hxx"

namespace Howard {

namespace Transform {

class Camera {
    public:

        void update();

        void set_pos(float x, float y, float z) {
            pos = { x, y, z }; }

        void set_look_at(float x, float y, float z) {
            look_at = { x, y, z }; }

        void set_vec_up(float x, float y, float z) {
            vec_up = { x, y, z }; }

        glm::vec3 pos { 384, 384, 320 };
        glm::vec3 look_at { 0, 0, 0 };
        glm::vec3 vec_up { 0, 0, 1 };
        glm::mat4 view_mat;

        float scale_factor = 0.6484375f;

        static Camera *instance;
};

HPoint view_pos(const HCoord& coord, const Camera *camera);

}

}

#endif // HOWARD11_TRANSFORM_HXX
