//
// Made by secondwtq <lovejay-lovemusic@outlook.com> with Love.
//
// Date: 2015-08-06
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of the HOWARD11 Game Engine.
//
// WE ARE STANDING ON THE EDGE.
//

#ifndef HOWARD11_JSFUNCTIONS_HXX
#define HOWARD11_JSFUNCTIONS_HXX

#include <string>

#include "thirdpt/mozjs.hxx"

std::string readfile(const std::string& filename);

bool js_print(JSContext *context, unsigned int argc, JS::Value *vp);
void js_collectgarbage(xoundation::spd::context_reference ctx);
bool js_cast(JSContext *ctx, unsigned int argc, JS::Value *vp);

#endif // HOWARD11_JSFUNCTIONS_HXX
