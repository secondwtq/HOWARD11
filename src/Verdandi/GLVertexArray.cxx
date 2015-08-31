//
// Created by secondwtq <lovejay-lovemusic@outlook.com> 2015/08/31.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of ISDCNext.
//
// We have always treaded the borderland.
//

#include "GLVertexArray.hxx"

#include "thirdpt/howardgl.hxx"

namespace Howard {
namespace Verdandi {

VertexArray::VertexArray() {
    GLuint t;
    glGenVertexArrays(1, &t);
    m_glid = t;
}

VertexArray::~VertexArray() {
    GLuint t = m_glid;
    glDeleteVertexArrays(1, &t);
    m_glid = 0;
}

VertexArrayScope::VertexArrayScope(VertexArray& array) {
    glBindVertexArray(array.id()); }

VertexArrayScope::~VertexArrayScope() {
    glBindVertexArray(0); }

}
}
