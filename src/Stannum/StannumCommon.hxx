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

#ifndef HOWARD11_STANNUMCOMMON_HXX
#define HOWARD11_STANNUMCOMMON_HXX

#include "Common.hxx"

namespace Howard {
namespace Stannum {

enum ShaderType {

    CommonSprite = 0,
    DuneTerrain = 1,
    Guardian = 2,
    GuardianCache = 3,
    End,

};

struct VertFormatSprite {
    glm::vec3 position;
    glm::vec3 texcoord;
    glm::vec2 location;
    glm::vec4 multiply;
};

}
}

#endif // HOWARD11_STANNUMCOMMON_HXX
