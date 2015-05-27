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

#include "GLFoundation.hxx"

#include "GLCommon.hxx"

#include "Misc/AtTheVeryBeginning.hxx"

namespace Howard {

namespace Verdandi {

VGLVertexArray vertex_arrays[5];

void reset_target() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, 1600, 1200);
//    glViewport(0, 0,
//            AtTheVeryBeginning::Setting<AtTheVeryBeginning::WindowSetting>::instance()->actual_width,
//            AtTheVeryBeginning::Setting<AtTheVeryBeginning::WindowSetting>::instance()
//                       ->actual_height);
}

void init_target() { return reset_target(); }

void gl_init() {

    glGenVertexArrays(5, &vertex_arrays[0]);
    glBindVertexArray(vertex_arrays[0]);

    glClearDepth(1.f);

    init_target();

}

}

}

