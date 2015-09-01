//
// Made by secondwtq <lovejay-lovemusic@outlook.com> with Love.
//
// Date: 2015-05-23
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of the HOWARD11 Game Engine.
//
// WE ARE STANDING ON THE EDGE.
//

#ifndef HOWARD11_COMMON_HXX
#define HOWARD11_COMMON_HXX

namespace Howard {

typedef unsigned char RawDataT;

enum HowardRTTIType {
    TBase = 0,
    TEventListener = 1,
    TNode = 2,
    THandle = 3,
    TAsset = 4,
    TEventQueueGlobalBase = 5,
    TEventQueueGlobal = 6,
    TEventNotificationCenter = 7
};

}

#define HO_INVALIDX -1
#define HO_HANDLE_NULL -1

#define HAS_FOUNDATION true
#define NO_FOUNDATION false

#define HO_ENDLINE "\n"

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

using HCoord = glm::i32vec3;
using HPoint = glm::i16vec2;
using HPixel = glm::u16vec2;
using HColor = glm::vec4;
using HAnyCoord = glm::vec3;
using HScale = glm::vec2;
using HQuaternion = glm::quat;

#define SHARED(type) std::shared_ptr<type>

#endif // HOWARD11_COMMON_HXX
