//
// Created by secondwtq <lovejay-lovemusic@outlook.com> 2015/09/01.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of ISDCNext.
//
// We have always treaded the borderland.
//

#include "GLHelper.hxx"

#include "GLCommon.hxx"
#include "thirdpt/howardgl.hxx"

namespace Howard {
namespace Verdandi {

VGLEnum Helper::glBufferUsage(BufferUsage usage) {
    switch (usage) {
        case UStatic:
            return GL_STATIC_DRAW;
        case UDynamic:
            return GL_DYNAMIC_DRAW;
        case UStream:
            return GL_STREAM_DRAW;
    }
}

}
}
