//
// Made by secondwtq <lovejay-lovemusic@outlook.com> with Love.
//
// Date: 2015-08-25
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of the HOWARD11 Game Engine.
//
// WE ARE STANDING ON THE EDGE.
//

#ifndef HOWARD11_DUNE_HXX
#define HOWARD11_DUNE_HXX

#include "../Common.hxx"

#include "DuneCommon.hxx"

#include "Stannum/StannumTexture.hxx"
#include "Verdandi/GLVertexArray.hxx"

#include <vector>
#include <memory>

namespace Howard {
namespace Dolly {
class Camera;
}
namespace Verdandi {
template <typename T>
class VertexBufferSingle;
}
namespace Stannum {
class StannumRenderer;
}
namespace Guardian {
class GuardianCanvas;
}

namespace Dune {

namespace Constants {

const size_t cellSize = 8;
const size_t gridSize = 2;

const size_t cellsPerChunkX = 16;
const size_t cellsPerChunkY = 16;

const size_t gridPerCellX = 4;
const size_t gridPerCellY = 4;

const size_t heightSpanMax = 192;

}

class DuneLayer;
class DuneChunk;
class DuneTextureCache;
struct DuneTextureCacheData;

}
}

namespace Howard {
namespace Dune {

// A terrain object has 16 x 16 chunks in maximum
//  that is, 256 x 256 cells, 1024 x 1024 polys (1048576)
//  (also the res of heightmap)
class DuneTerrain {
public:

    // 150901: we supports non-zero origin now with setOrigin(),
    //  but since most of cases we just constructs it at zero
    //  so we donot provide it as a parameter in ctor, though
    //  it calls setOrigin() with default { };
    DuneTerrain(const HPixel& num_chunks,
            Stannum::StannumRenderer *renderer);

    inline SHARED(Verdandi::VertexBufferSingle<VertFormatDuneTerrain>)
        vertexBuffer() { return m_vert_buffer; }

    void setHeightmap(SHARED(Verdandi::TextureImage) heightmap);
    inline SHARED(Verdandi::TextureImage) heightmap() {
        return m_heightmap; }

    void cacheChunk(std::shared_ptr<DuneChunk> chunk);

    inline HPixel totalSize() const {
        return m_total_size; }

    HAnyCoord heightfieldScale() const;

    inline HPixel sizePerChunk2D() const {
        const static HPixel ret { Constants::cellsPerChunkX * Constants::cellSize,
            Constants::cellsPerChunkY * Constants::cellSize };
        return ret;
    }

    inline SHARED(DuneChunk) chunkAt(size_t x, size_t y) {
        ASSERT(x < m_total_size.x && y < m_total_size.y);
        return m_chunks[x][y];
    }

    inline HAnyCoord origin() const {
        return m_origin; }
    // this, effectively, sets the position
    //  of it's chunks
    //
    //  and is called by the ctor after
    //  the chunks created
    void setOrigin(const HAnyCoord& origin);

    HPixel m_num_chunks;
    std::vector<std::vector<SHARED(DuneChunk)>> m_chunks;
    HPixel m_total_size;
    SHARED(Verdandi::TextureImage) m_heightmap;
    SHARED(Verdandi::VertexBufferSingle<VertFormatDuneTerrain>) m_vert_buffer;
    std::vector<SHARED(DuneTextureCache)> m_caches;
    Verdandi::VertexArray m_vao;

    SHARED(Verdandi::Image) m_scaled_height;

    Stannum::StannumRenderer *m_renderer;

private:

    HAnyCoord m_origin;
    void initializeVAO();
};

// According to our documents
// A chunk has 16 x 16 cells (256 in total),
//  64 x 64 polys (4096 in total, and 8192 tris)
class DuneChunk : public std::enable_shared_from_this<DuneChunk> {
public:

    DuneChunk(DuneTerrain *parent) :
            DuneChunk(parent, { }) { }
    DuneChunk(DuneTerrain *parent, const HAnyCoord& position);

    inline bool cached() const {
        return !m_cache_data.expired(); }
    inline void cache() {
        ASSERT(!cached());
        m_parent->cacheChunk(shared_from_this());
    }

    inline void ensureCached() {
        if (!cached()) {
            cache(); }
    }

    // world position (in meters) of left-top corner
    //  150901: changed from HPixel to HAnyCoord
    inline HAnyCoord position() const {
        return m_position; }
    void setPosition(const HAnyCoord& position);

    inline std::shared_ptr<DuneTextureCacheData> cacheData() {
        ASSERT(cached());
        return m_cache_data.lock();
    }

    void cacheMarkUsed();

    inline void cacheMarkUsedIfCached() {
        if (cached()) {
            cacheMarkUsed(); }
    }

    const std::vector<std::shared_ptr<DuneLayer>>& layers() const {
        return m_layers; }

    inline size_t numberOfLayers() const {
        return m_layers.size(); }

    inline void appendLayer(std::shared_ptr<DuneLayer> layer) {
        ASSERT(layer != nullptr);
        m_layers.push_back(layer);
    }

    inline SHARED(DuneLayer) layerAt(size_t idx) {
        ASSERT(idx < numberOfLayers());
        return m_layers[idx];
    }

    void updateCachedTexture();

private:

    std::vector<std::shared_ptr<DuneLayer>> m_layers;

    friend struct DuneTextureCacheData;
    inline void setCacheData(std::weak_ptr<DuneTextureCacheData> data) {
        m_cache_data = data; }

    std::weak_ptr<DuneTextureCacheData> m_cache_data;
    DuneTerrain *m_parent;
    // NOTE: refering to the position of left-top corner
    HAnyCoord m_position;
};

class DuneTextureSet {
public:

    std::shared_ptr<Verdandi::TextureImage> texture(DuneTextureType type) {
        return m_textures[type]; }

    DuneTextureSet& setTexture(DuneTextureType type,
            SHARED(Verdandi::TextureImage) texture) {
        m_textures[type] = texture; return *this; }

    std::shared_ptr<Verdandi::TextureImage> m_textures[DuneTextureType::DEnd];
};

class DuneLayer {
public:

    DuneLayer(std::shared_ptr<DuneTextureSet> textures,
            std::shared_ptr<Verdandi::TextureImage> mask)
            : m_textures(textures), m_mask(mask) {
        ASSERT((textures != nullptr) && (mask != nullptr)); }

    const std::shared_ptr<DuneTextureSet> textureSet() const {
        return m_textures; }

    const std::shared_ptr<Verdandi::TextureImage> mask() const {
        return m_mask; }

    HAnyCoord uvscale { 2, 1, 2 };
    std::shared_ptr<DuneTextureSet> m_textures;
    std::shared_ptr<Verdandi::TextureImage> m_mask;
};

class Helper {
public:
    static std::vector<VertFormatDuneTerrain> generateChunkVertex(
            HPixel count_faces, float cell_size);
};

}
}

#endif // HOWARD11_DUNE_HXX
