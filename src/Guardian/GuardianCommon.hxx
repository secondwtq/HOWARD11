//
// Made by secondwtq <lovejay-lovemusic@outlook.com> with Love.
//
// Date: 2015-08-28
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of the HOWARD11 Game Engine.
//
// WE ARE STANDING ON THE EDGE.
//

#ifndef HOWARD11_GUARDIANCOMMON_HXX
#define HOWARD11_GUARDIANCOMMON_HXX

#include <glm/glm.hpp>

namespace Howard {
namespace Guardian {

struct VertFormatGuardianGeneral {

    VertFormatGuardianGeneral(const glm::vec2& p, const glm::vec2& t)
            : position(p), texcoord(t) { }

    glm::vec2 position;
    glm::vec2 texcoord;
};

}
}

#endif // HOWARD11_GUARDIANCOMMON_HXX
