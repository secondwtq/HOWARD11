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

#include "Stannum/Stannum.hxx"
#include "Stannum/StannumTexture.hxx"
#include "Misc/Transform.hxx"
#include "Verdandi/GLVertexArray.hxx"

#include <vector>
#include <memory>
#include <map>
#include <unordered_map>
#include <list>

namespace Howard {

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

}

enum DuneTextureType {
    DColor,
    DNormal,
    DSpecular,
    DEnd
};

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

    void initializeVAO();
};

// According to our documents
// A chunk has 16 x 16 cells (256 in total),
//  64 x 64 polys (4096 in total, and 8192 tris)
class DuneChunk : public std::enable_shared_from_this<DuneChunk> {
public:

    DuneChunk(DuneTerrain *parent, const HPixel& position);

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
    inline HPixel position() const {
        return m_position; }

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
    HPixel m_position;
};

struct DuneTextureCacheData {
    std::weak_ptr<DuneChunk> chunk;
    std::weak_ptr<DuneTextureCache> cache;

    inline const glm::u8vec2& index() const {
        return m_index; }

    // for the uniform vec2 *cache_position* in
    //  Dune shader, we accessorized these properties
    inline const glm::vec2& texcoord() const {
        return m_texcoord; }

    inline void setIndex(const glm::u8vec2& idx) {
        m_texcoord = (glm::vec2) idx / 8.0f;
        m_index = idx; }

    inline void updateChunkWithSelf(std::weak_ptr<DuneTextureCacheData> self) {
        ASSERT(!chunk.expired());
        ASSERT(!self.expired() && self.lock().get() == this);
        chunk.lock()->setCacheData(self);
    }

private:
    glm::vec2 m_texcoord;
    glm::u8vec2 m_index;
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

class DispatchCommandDuneTerrain : public Stannum::StannumDispatchCommand {
public:

    DispatchCommandDuneTerrain(DuneTerrain *terrain, Dolly::Camera *camera)
            : m_terrain(terrain), m_camera(camera) { }

    Stannum::DispatchCommandType commandType() const override {
        return Stannum::DispatchCommandType::DDuneTerrain; }

    void execute(Stannum::StannumRenderer *renderer) override;

private:
    DuneTerrain *m_terrain;
    Dolly::Camera *m_camera;
};

}
}

#endif // HOWARD11_DUNE_HXX
