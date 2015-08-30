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

#include <vector>
#include <memory>
#include <unordered_map>
#include <deque>

namespace Howard {
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

class DuneChunk;
class DuneTextureCache;
struct DuneTextureCacheData;

// A terrain object has 16 x 16 chunks in maximum
//  that is, 256 x 256 cells, 1024 x 1024 polys (1048576)
//  (also the res of heightmap)
class DuneTerrain {
public:

    DuneTerrain(const HPixel& num_chunks);

    inline std::shared_ptr<Verdandi::VertexBufferSingle<VertFormatDuneTerrain>> vertexBuffer() {
        return m_vert_buffer; }

    void setHeightmap(std::shared_ptr<Verdandi::TextureImage> heightmap);

    inline std::shared_ptr<Verdandi::TextureImage> heightmap() {
        return m_heightmap; }

    void cacheChunk(std::shared_ptr<DuneChunk> chunk);

    inline HPixel totalSize() const {
        return m_total_size; }

    HAnyCoord heightfieldScale() const;

    HPixel m_num_chunks;
    std::vector<std::vector<std::shared_ptr<DuneChunk>>> m_chunks;
    HPixel m_total_size;
    std::shared_ptr<Verdandi::TextureImage> m_heightmap;
    std::shared_ptr<Verdandi::VertexBufferSingle<VertFormatDuneTerrain>> m_vert_buffer;
    std::vector<std::shared_ptr<DuneTextureCache>> m_caches;

    std::shared_ptr<Verdandi::Image> m_scaled_height;
};

class DuneTextureCache : public std::enable_shared_from_this<DuneTextureCache> {
public:

    DuneTextureCache();

    inline size_t size() const {
        return m_cache_cache.size(); }
    inline bool full() const {
        return size() >= 64; }
    inline bool empty() const {
        return size() == 0; }

    bool hasCacheFor(std::shared_ptr<DuneChunk> chunk) const;
    bool entryHold(const glm::u8vec2& idx) const;
    const std::weak_ptr<DuneTextureCacheData> cacheDataFor(std::shared_ptr<DuneChunk> chunk) const;
    inline std::shared_ptr<Verdandi::TextureImage> *textures() {
        return &m_textures[0]; }

    std::shared_ptr<DuneTextureCacheData> pickAndKickAnEntry();
    // calling this when cache is full leads to an assertion failed
    glm::u8vec2 pickAnEntryWhenNotFull() const;

    std::weak_ptr<DuneTextureCacheData> insertCacheEntry(
            std::shared_ptr<DuneChunk> chunk, const glm::u8vec2& idx);

private:

    std::deque<std::shared_ptr<DuneTextureCacheData>> m_queue;
    std::weak_ptr<DuneTextureCacheData> m_caches[8][8];
    std::unordered_map<DuneChunk *, std::weak_ptr<DuneTextureCacheData>> m_cache_cache;

    std::shared_ptr<Verdandi::TextureImage> m_textures[DuneTextureType::DEnd];
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
        if (!cached()) {
            m_parent->cacheChunk(shared_from_this()); }
    }

    // world position (in meters) of left-top corner
    inline HPixel position() const {
        return m_position; }

    inline std::shared_ptr<DuneTextureCacheData> cacheData() {
        ASSERT(cached());
        return m_cache_data.lock();
    }

private:
    friend struct DuneTextureCacheData;
    inline void setData(std::weak_ptr<DuneTextureCacheData> data) {
        m_cache_data = data; }

    std::weak_ptr<DuneTextureCacheData> m_cache_data;
    DuneTerrain *m_parent;
    // NOTE: refering to the position of left-top corner
    HPixel m_position;
};

struct DuneTextureCacheData {
    std::weak_ptr<DuneChunk> chunk;
    std::weak_ptr<DuneTextureCache> cache;

    glm::u8vec2 index;

    inline void updateChunkWithSelf(std::weak_ptr<DuneTextureCacheData> self) {
        ASSERT(!chunk.expired());
        ASSERT(!self.expired() && self.lock().get() == this);
        chunk.lock()->setData(self);
    }
};

class DuneLayer {
public:

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
