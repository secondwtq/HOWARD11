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

#include "DuneTextureCache.hxx"

#include "DuneCommon.hxx"
#include "Dune.hxx"
#include "Debug.hxx"
#include "Stannum/StannumTexture.hxx"
#include "Guardian/Guardian.hxx"

#include "FSM/FSM.hxx"

#include <glm/glm.hpp>

#include <string>
#include <memory>
#include <list>
#include <unordered_map>

namespace Howard {
namespace Dune {

using namespace FSMHelper;

DuneTextureCache::DuneTextureCache(Stannum::StannumRenderer *renderer)
        : m_renderer(renderer), m_canvas(new Guardian::GuardianCanvas(renderer)) {
    ASSERT(renderer);
    for (size_t i = DuneTextureType::DColor; i < DuneTextureType::DEnd; i++) {
        std::string name = "DuneTextureCache_"; name += i;
        this->m_textures[i] = std::make_shared<Verdandi::TextureImage>(name);
        this->m_textures[i]->loadEmpty({ 4096, 4096 }, Verdandi::ImageChannelType::IRGB);
		m_textures[i]->id();
    }
    initializeCanvas();
}

bool DuneTextureCache::hasCacheFor(std::shared_ptr<DuneChunk> ptr) const {
    auto i = m_cache_cache.find(ptr.get());
    return i != m_cache_cache.end() && !i->second.expired();
}

bool DuneTextureCache::entryHold(const glm::u8vec2& idx) const {
    ASSERT(idx.x <= 8 && idx.y <= 8);

    return !m_caches[idx.x][idx.y].expired();
}

const std::weak_ptr<DuneTextureCacheData> DuneTextureCache::cacheDataFor(std::shared_ptr<DuneChunk> chunk) const {
    auto i = m_cache_cache.find(chunk.get());
    if (i != m_cache_cache.end() && !i->second.expired()) {
        return i->second; }
    return std::weak_ptr<DuneTextureCacheData>();
}

std::weak_ptr<DuneTextureCacheData> DuneTextureCache::insertCacheEntry(
        std::shared_ptr<DuneChunk> chunk, const glm::u8vec2& idx) {
    ASSERT(idx.x <= 8 && idx.y <= 8);
    ASSERT(chunk != nullptr);

    ASSERT(!hasCacheFor(chunk) && !entryHold(idx));
    log("Dune", L::Message) << "DuneTextureCache::insertCacheEntry - inserting to ("
    << idx.x << ", " << idx.y << ")" << rn;

    std::shared_ptr<DuneTextureCacheData> data = std::make_shared<DuneTextureCacheData>();
    data->setIndex(idx);
    data->chunk = chunk;
    data->cache = shared_from_this();

    auto i = m_lru.insert(m_lru.end(), data);
    m_lru_map.insert(std::make_pair(data, i));

    m_caches[idx.x][idx.y] = data;
    m_cache_cache.insert({ chunk.get(), data });
    data->updateChunkWithSelf(data);

    chunk->updateCachedTexture();

    return data;
}

std::shared_ptr<DuneTextureCacheData> DuneTextureCache::pickAndKickAnEntry() {
    std::shared_ptr<DuneTextureCacheData> ret;
    if (empty()) {
        ret->setIndex(pickAnEntryWhenNotFull());
    } else {
        ASSERT(!m_lru.empty());
        std::shared_ptr<DuneTextureCacheData> data = m_lru.front().lock();
        // you use memcpy()? fork you
        ret = std::make_shared<DuneTextureCacheData>(*data);
        {
            ASSERT(!ret->chunk.expired());
            DuneChunk *chunk = ret->chunk.lock().get();
            auto i = m_cache_cache.find(chunk);
            ASSERT(i != m_cache_cache.end() && !i->second.expired());
            m_cache_cache.erase(i);

            ASSERT(entryHold(ret->index()));
            m_caches[ret->index().x][ret->index().y] = std::weak_ptr<DuneTextureCacheData>();
        }
        // if the only shared_ptr disappeared,
        //  the weak_ptr in DuneChunk would hopefully expire.
        log("Dune", L::Message) << "DuneTextureCache::pickAndKickAnEntry - kicking ("
        << ret->index().x << ", " << ret->index().y << ")" << rn;

        const auto i = m_lru_map.find(data);
        ASSERT(i != m_lru_map.end());
        m_lru.pop_front();
        m_lru_map.erase(i);
    }
    return ret;
}

void DuneTextureCache::markCacheEntryUsed(
        const std::weak_ptr<DuneTextureCacheData>& ptr) {
    ASSERT(!ptr.expired());
    const auto i = m_lru_map.find(ptr.lock());
    ASSERT(i != m_lru_map.end());

    m_lru.splice(m_lru.end(), m_lru, i->second);
}

glm::u8vec2 DuneTextureCache::pickAnEntryWhenNotFull() const {
    ASSERT(!full());
    // TODO: just a brute force
    for (size_t i = 0; i < 8; i++) {
        for (size_t j = 0; j < 8; j++) {
            if (!entryHold({ i, j })) {
                log("Dune", L::Message) << "DuneTextureCache::pickAnEntryWhenNotFull - picked (" << i << ", " << j << ")" << rn;
                return { i, j }; }
        }
    }
    ASSERT_FOUNDATION();
    return { 0, 0 };
}

void DuneTextureCacheData::updateChunkWithSelf(std::weak_ptr<DuneTextureCacheData> self) {
    ASSERT(!chunk.expired());
    ASSERT(!self.expired() && self.lock().get() == this);
    chunk.lock()->setCacheData(self);
}

}
}
