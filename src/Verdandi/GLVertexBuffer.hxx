//
// Created by secondwtq <lovejay-lovemusic@outlook.com> 2015/08/28.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of ISDCNext.
//
// We have always treaded the borderland.
//

#ifndef HOWARD11_GLVERTEXBUFFER_HXX
#define HOWARD11_GLVERTEXBUFFER_HXX

#include "GLCommon.hxx"
#include "GLFoundation.hxx"

#include "Debug.hxx"

#include <vector>
#include <memory>

namespace Howard {
namespace Verdandi {

class Helper {
public:
    inline static GLenum glBufferUsage(BufferUsage usage) {
        switch (usage) {
            case UStatic:
                return GL_STATIC_DRAW;
            case UDynamic:
                return GL_DYNAMIC_DRAW;
            case UStream:
                return GL_STREAM_DRAW;
        }
    }
};

template <typename T>
class VertexBufferScope {
public:
    VertexBufferScope(T& buffer) {
        glBindBuffer(GL_ARRAY_BUFFER, buffer.id()); }

    VertexBufferScope(std::shared_ptr<T> buffer) {
        glBindBuffer(GL_ARRAY_BUFFER, buffer->id()); }

    ~VertexBufferScope() {
        glBindBuffer(GL_ARRAY_BUFFER, 0); }
};

class VertexAttributeSimpleScope {
public:
    VertexAttributeSimpleScope(VGLIDX id) : m_id(id) {
        glEnableVertexAttribArray(m_id); }

    ~VertexAttributeSimpleScope() {
        glDisableVertexAttribArray(m_id); }

private:
    VGLIDX m_id;
};

template <typename T>
class VertexBufferSingle {
private:
    struct shared_c { };

public:

    static inline std::shared_ptr<VertexBufferSingle<T>> createWithVector(
            BufferUsage usage, const std::vector<T>& data) {
        return createWithData(usage, data.data(), data.size());
    }

    static std::shared_ptr <VertexBufferSingle<T>> createWithData(
            BufferUsage usage, const T *ptr, size_t count) {
        ASSERT(ptr);
        ASSERT(count);

        std::shared_ptr <VertexBufferSingle<T>> ret =
                std::make_shared<VertexBufferSingle<T>>(shared_c());
        glGenBuffers(1, &ret->m_glid);
        {
            VertexBufferScope<VertexBufferSingle<T>> buffer(ret);
            glBufferData(GL_ARRAY_BUFFER, count * sizeof(T), ptr, Helper::glBufferUsage(usage));
        }
        ret->m_count = count;
        return ret;
    }

    inline VGLIDX id() { return m_glid; }

    size_t countElements() const {
        return m_count;
    }

    explicit VertexBufferSingle(const shared_c&) { }

    ~VertexBufferSingle() {
        glDeleteBuffers(1, &m_glid);
    }

private:

    VertexBufferSingle();

    VGLIDX m_glid = 0;
    size_t m_count = 0;
};

}
}

#endif // HOWARD11_GLVERTEXBUFFER_HXX
