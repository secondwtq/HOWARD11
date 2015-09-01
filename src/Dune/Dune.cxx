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

#include "Dune.hxx"

#include "DuneCommon.hxx"
#include "DuneShader.hxx"

#include "Verdandi/GLVertexBuffer.hxx"
#include "Verdandi/GLVertexArray.hxx"
#include "Misc/AtTheVeryBeginning.hxx"
#include "FSM/FSM.hxx"
#include "FSM/FSMHelper.hxx"

#include "Guardian/Guardian.hxx"
#include "Misc/DuneFrustum.hxx"

#include <vector>
#include <glm/gtc/matrix_transform.hpp>

namespace Howard {

using namespace FSMHelper;

namespace Dune {

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

std::vector<VertFormatDuneTerrain> Helper::generateChunkVertex(
        HPixel count_faces, float cell_size) {
    std::vector<VertFormatDuneTerrain> ret;
    for (size_t i = 0; i < count_faces.x; i++) {
        for (size_t j = 0; j < count_faces.y; j++) {
            ret.emplace_back(glm::vec2(i * cell_size, j * cell_size));
            ret.emplace_back(glm::vec2(i * cell_size, (j+1) * cell_size));
            ret.emplace_back(glm::vec2((i+1) * cell_size, (j+1) * cell_size));

            ret.emplace_back(glm::vec2(i * cell_size, j * cell_size));
            ret.emplace_back(glm::vec2((i+1) * cell_size, (j+1) * cell_size));
            ret.emplace_back(glm::vec2((i+1) * cell_size, j * cell_size));
        }
    }
    return ret;
}

DuneTerrain::DuneTerrain(const HPixel& num_chunks, Stannum::StannumRenderer *renderer) :
        m_num_chunks(num_chunks), m_chunks(num_chunks.x),
        m_total_size(num_chunks.x * Constants::cellsPerChunkX * Constants::cellSize,
            num_chunks.y * Constants::cellsPerChunkY * Constants::cellSize),
        m_vert_buffer(Verdandi::VertexBufferSingle<VertFormatDuneTerrain>
        ::createWithVector(Verdandi::BufferUsage::UStatic, Helper::generateChunkVertex(
                { Constants::cellsPerChunkX * Constants::gridPerCellX,
                        Constants::cellsPerChunkY * Constants::gridPerCellY },
                Constants::gridSize))),
        m_renderer(renderer) {

    ASSERT(renderer);

    for (size_t i = 0; i < num_chunks.x; i++) {
        for (size_t j = 0; j < num_chunks.y; j++) {
            m_chunks[i].emplace_back(std::make_shared<DuneChunk>(this,
                    HPixel(i * Constants::cellsPerChunkX * Constants::cellSize,
                            j * Constants::cellsPerChunkY * Constants::cellSize)));
        }
    }

    m_caches.emplace_back(std::make_shared<DuneTextureCache>(renderer));
    initializeVAO();

}

void DuneTerrain::initializeVAO() {
    using namespace Verdandi;

    VertexArrayScope scope(m_vao);
    VertexBufferScope<VertexBufferSingle<VertFormatDuneTerrain>>
            buf(vertexBuffer());
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
            sizeof(VertFormatDuneTerrain), nullptr);
}

DuneChunk::DuneChunk(DuneTerrain *parent, const HPixel& position) :
        m_parent(parent), m_position(position) { }

void DuneChunk::cacheMarkUsed() {
    ASSERT(cached());
    cacheData()->cache.lock()->markCacheEntryUsed(cacheData());
}

void DispatchCommandDuneTerrain::execute(Stannum::StannumRenderer *renderer) {
    using namespace Stannum;
    using namespace Verdandi;
    using namespace AtTheVeryBeginning;

    glm::mat4 view_projection;
    {
        glm::mat4 projection = glm::ortho(0.f, (float) Setting<WindowSetting>::instance()->actual_width,
                (float) Setting<WindowSetting>::instance()->actual_height, 0.f, -16384.f, 16384.f);
        view_projection = scale(projection, glm::vec3 { 1, 1, 1 } * m_camera->scale_factor) * m_camera->view_mat;
    }

    std::vector<DuneChunk *> chunks_to_render;
    Dolly::FrustumBox frustum = Dolly::FrustumBox::createFromMatrix(view_projection);
    for (auto chunks : m_terrain->m_chunks) {
        for (auto chunk : chunks) {
            // TODO: more accurate AABB, maybe with physics?
            HAnyCoord center { (glm::vec2) chunk->position() +
                    (glm::vec2) m_terrain->sizePerChunk2D() / 2.0f, 192 / 2.0f };
            HAnyCoord half_extends { (glm::vec2) m_terrain->sizePerChunk2D() / 2.0f, 192 / 2.0f };
            if (frustum.checkAABB(center, half_extends)) {
                chunks_to_render.push_back(chunk.get()); }
        }
    }

    size_t numChunkToRender = chunks_to_render.size(),
        numChunkCached = 0, numChunkToCache = 0;
    for (auto chunk : chunks_to_render) {
        if (chunk->cached()) {
            numChunkCached++;
            chunk->cacheMarkUsed();
        }
    }
    for (auto chunk : chunks_to_render) {
        if (!chunk->cached()) {
            numChunkToCache++;
            chunk->cache();
        }
    }

    auto *shader = renderer->shaders()->get_shader
            <DuneTerrainShader>(ShaderType::DuneTerrain);
    shader->use();
    VertexArrayScope vao(m_terrain->m_vao);

    SET_UNIFORMAT4P(shader, model_view_and_projection, view_projection);
    if (m_terrain->heightmap() && m_terrain->heightmap()->ok()) {
        BIND_TEXTUREP(shader, heightmap, m_terrain->heightmap()->id(), 0); }
    SET_UNIFORM2P(shader, total_size, m_terrain->m_total_size);
    SET_UNIFORM2P(shader, chunk_size, m_terrain->sizePerChunk2D());

    // TODO: culling, without which the cache
    //  just does not work
    // TODO: instancing
    for (auto chunk : chunks_to_render) {
        // anyone has something like "multicache"?
        //  this assert surprisingly works for FIFO solution
        ASSERT(chunk->cached());

        SET_UNIFORM2P(shader, chunk_position, chunk->position());
        SET_UNIFORM2P(shader, cache_position, chunk->cacheData()->texcoord());
        BIND_TEXTUREP(shader, texcache_diffuse,
                    chunk->cacheData()->cache.lock()->textures()[DuneTextureType::DColor]->id(), 1);
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(
                m_terrain->vertexBuffer()->countElements()));
    }
}

DuneTextureCache::DuneTextureCache(Stannum::StannumRenderer *renderer)
    : m_renderer(renderer), m_canvas(new Guardian::GuardianCanvas(renderer)) {
    ASSERT(renderer);
    for (size_t i = DuneTextureType::DColor; i < DuneTextureType::DEnd; i++) {
        std::string name = "DuneTextureCache_"; name += i;
        this->m_textures[i] = std::make_shared<Verdandi::TextureImage>(name);
        this->m_textures[i]->loadEmpty({ 4096, 4096 }, Verdandi::ImageChannelType::IRGB);
    }
    initializeCanvas();
}

void DuneTextureCache::initializeCanvas() {
    m_canvas->setTarget(this->m_textures[0]); }

void DuneTerrain::cacheChunk(std::shared_ptr<DuneChunk> chunk) {
    ASSERT(chunk);
    ASSERT(!chunk->cached());

    std::shared_ptr<DuneTextureCache> cache = m_caches[0];
    glm::u8vec2 pos;
    if (!cache->full()) {
        pos = cache->pickAnEntryWhenNotFull();
    } else {
        pos = cache->pickAndKickAnEntry()->index();
    }
    cache->insertCacheEntry(chunk, pos);
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

void DuneChunk::updateCachedTexture() {
    ASSERT(cached());
    std::shared_ptr<DuneTextureCacheData> data = m_cache_data.lock();
    std::shared_ptr<Guardian::GuardianElementTerrainLayersGroup> element =
            std::make_shared<Guardian::GuardianElementTerrainLayersGroup>(data->index());
    for (auto layer : layers()) {
        element->addLayer(layer); }
//    if (element->numberOfLayers()) {
        data->cache.lock()->canvas()->paintBlock([ &element ](Guardian::GuardianCanvas *canvas) {
            element->paint(canvas); });
//    }
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

void DuneTerrain::setHeightmap(std::shared_ptr<Verdandi::TextureImage> heightmap) {
    m_heightmap = heightmap;

    if (m_heightmap) {
        m_scaled_height = std::make_shared<Verdandi::Image>("terrain_scaledheight");
        m_scaled_height->loadFromTextureImage(*m_heightmap.get());
        m_scaled_height->scale({
                1.0 / Constants::gridPerCellX, 1.0 / Constants::gridPerCellY });
    } else {
        m_scaled_height.reset();
    }
}

HAnyCoord DuneTerrain::heightfieldScale() const {
    ASSERT(m_heightmap && m_heightmap->ok());
    ASSERT(m_scaled_height && m_scaled_height->ok());

    return { totalSize().x / m_scaled_height->size.x,
            totalSize().y / m_scaled_height->size.y,
        192.0 / 256.0 };
}

}
}
