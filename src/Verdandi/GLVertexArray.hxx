//
// Created by secondwtq <lovejay-lovemusic@outlook.com> 2015/08/31.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of ISDCNext.
//
// We have always treaded the borderland.
//

#ifndef HOWARD11_GLVERTEXARRAY_HXX
#define HOWARD11_GLVERTEXARRAY_HXX

#include "Common.hxx"
#include "GLCommon.hxx"

#include <memory>

namespace Howard {
namespace Verdandi {

class VertexArray {
public:

    VertexArray();
    ~VertexArray();

    inline VGLIDX id() {
        return m_glid; }

private:
    VGLIDX m_glid;
};

class VertexArrayScope {
public:
    VertexArrayScope(VertexArray& array);
    VertexArrayScope(SHARED(VertexArray) array)
            : VertexArrayScope(*array) { }
    ~VertexArrayScope();
};

}
}

#endif // HOWARD11_GLVERTEXARRAY_HXX
