//
// Created by secondwtq <lovejay-lovemusic@outlook.com> 2015/08/25.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of ISDCNext.
//
// We have always treaded the borderland.
//

#ifndef HOWARD11_DUNE_HXX
#define HOWARD11_DUNE_HXX

#include "../Common.hxx"

#include "DuneCommon.hxx"

#include "Stannum/Stannum.hxx"
#include "Stannum/StannumTexture.hxx"

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

    ~VertexBufferScope() { glBindBuffer(GL_ARRAY_BUFFER, 0); }
};

template <typename T>
class VertexBufferSingle {
private:
    struct shared_c { };

public:

    static inline std::shared_ptr<VertexBufferSingle<T>> createWithVector(
            BufferUsage usage, const std::vector<T>& data) {
        return createWithData(usage, data.data(), data.size()); }

    static std::shared_ptr<VertexBufferSingle<T>> createWithData(
            BufferUsage usage, const T *ptr, size_t count) {
        ASSERT(ptr);
        ASSERT(count);

        std::shared_ptr<VertexBufferSingle<T>> ret =
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

    explicit VertexBufferSingle(const shared_c&) { }

    ~VertexBufferSingle() {
        glDeleteBuffers(1, &m_glid); }

private:

    VertexBufferSingle();

    VGLIDX m_glid = 0;
    size_t m_count = 0;
};

}

namespace Dune {

namespace Constants {

const size_t cellSize = 8;

const size_t cellsPerChunkX = 16;
const size_t cellsPerChunkY = 16;

}

class DuneChunk;

// A terrain object has 16 x 16 chunks in maximum
//  that is, 256 x 256 cells, 1024 x 1024 polys (1048576)
//  (also the res of heightmap)
class DuneTerrain {
public:

    DuneTerrain(const HPixel& num_chunks);

    HPixel m_num_chunks;
    std::vector<std::vector<std::shared_ptr<DuneChunk>>> m_chunks;
    std::shared_ptr<Verdandi::TextureImage> m_heightmap;
    std::shared_ptr<Verdandi::VertexBufferSingle<VertFormatDuneTerrain>> m_vert_buffer;
};

// According to our documents
// A chunk has 16 x 16 cells (256 in total),
//  64 x 64 polys (4096 in total, and 8192 tris)
class DuneChunk {
public:

    DuneChunk(DuneTerrain *parent, const HPixel& position);

    DuneTerrain *m_parent;
    // NOTE: refering to the position of left-top corner
    HPixel m_position;
};

class Helper {
public:

    static std::vector<VertFormatDuneTerrain> generateChunkVertex(
            HPixel count_faces, float cell_size);

};

}
}

#endif // HOWARD11_DUNE_HXX
