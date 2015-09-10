//
// Made by secondwtq <lovejay-lovemusic@outlook.com> with Love.
//
// Date: 2015-09-01
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of the HOWARD11 Game Engine.
//
// WE ARE STANDING ON THE EDGE.
//

#ifndef HOWARD11_DUNETEXTURECACHE_HXX
#define HOWARD11_DUNETEXTURECACHE_HXX

#include "DuneCommon.hxx"

// this is an, somewhat,
//  'implementation only' file
//
//  reference: timday.bitbucket.org/lru.html
#include <glm/glm.hpp>
#include <memory>
#include <list>
#include <unordered_map>

namespace Howard {
namespace Verdandi {
class TextureImage;
}
namespace Stannum {
class StannumRenderer;
}
namespace Guardian {
class GuardianCanvas;
}
namespace Dune {

class DuneChunk;
struct DuneTextureCacheData;

class DuneTextureCache : public std::enable_shared_from_this<DuneTextureCache> {
public:

    DuneTextureCache(Stannum::StannumRenderer *renderer);

    void initializeCanvas();

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

    void markCacheEntryUsed(const std::weak_ptr<DuneTextureCacheData>& data);

    inline Guardian::GuardianCanvas *canvas() {
        return m_canvas; }

private:

    std::list<std::weak_ptr<DuneTextureCacheData>> m_lru;
    std::unordered_map<std::shared_ptr<DuneTextureCacheData>,
    std::list<std::weak_ptr<DuneTextureCacheData>>::iterator> m_lru_map;

    std::weak_ptr<DuneTextureCacheData> m_caches[8][8];
    std::unordered_map<DuneChunk *, std::weak_ptr<DuneTextureCacheData>> m_cache_cache;

    std::shared_ptr<Verdandi::TextureImage> m_textures[DuneTextureType::DEnd];

    Stannum::StannumRenderer *m_renderer;
    Guardian::GuardianCanvas *m_canvas;
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

    void updateChunkWithSelf(std::weak_ptr<DuneTextureCacheData> self);

private:
    glm::vec2 m_texcoord;
    glm::u8vec2 m_index;
};

}
}

#endif // HOWARD11_DUNETEXTURECACHE_HXX
