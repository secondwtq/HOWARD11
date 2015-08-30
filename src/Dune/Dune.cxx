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
#include "Misc/AtTheVeryBeginning.hxx"
#include "FSM/FSM.hxx"
#include "FSM/FSMHelper.hxx"

#include <vector>
#include <glm/gtc/matrix_transform.hpp>

namespace Howard {

using namespace FSMHelper;

namespace Dune {

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

DuneTerrain::DuneTerrain(const HPixel& num_chunks) :
        m_num_chunks(num_chunks), m_chunks(num_chunks.x),
        m_total_size(num_chunks.x * Constants::cellsPerChunkX * Constants::cellSize,
            num_chunks.y * Constants::cellsPerChunkY * Constants::cellSize),
        m_vert_buffer(Verdandi::VertexBufferSingle<VertFormatDuneTerrain>
        ::createWithVector(Verdandi::BufferUsage::UStatic, Helper::generateChunkVertex(
                { Constants::cellsPerChunkX * Constants::gridPerCellX,
                        Constants::cellsPerChunkY * Constants::gridPerCellY },
                Constants::gridSize))) {

    for (size_t i = 0; i < num_chunks.x; i++) {
        for (size_t j = 0; j < num_chunks.y; j++) {
            m_chunks[i].emplace_back(std::make_shared<DuneChunk>(this,
                    HPixel(i * Constants::cellsPerChunkX * Constants::cellSize,
                            j * Constants::cellsPerChunkY * Constants::cellSize)));
        }
    }

    m_caches.emplace_back(std::make_shared<DuneTextureCache>());

}

DuneChunk::DuneChunk(DuneTerrain *parent, const HPixel& position) :
        m_parent(parent), m_position(position) { }

void DispatchCommandDuneTerrain::execute(Stannum::StannumRenderer *renderer) {
    using namespace Stannum;
    using namespace Verdandi;
    using namespace AtTheVeryBeginning;

    {
        for (auto chunks : m_terrain->m_chunks) {
            for (auto chunk : chunks) {
                if (!chunk->cached()) {
                    chunk->cache();
                }
            }
        }
    }

    auto *shader = renderer->shaders()->get_shader
            <DuneTerrainShader>(ShaderType::DuneTerrain);
    shader->use();
    VertexBufferScope<VertexBufferSingle<VertFormatDuneTerrain>>
                buf(m_terrain->vertexBuffer());
    VertexAttributeSimpleScope attr0(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(VertFormatDuneTerrain), nullptr);

    {
        glm::mat4 view = m_camera->view_mat;
        glm::mat4 projection = glm::ortho(0.f, (float) Setting<WindowSetting>::instance()->actual_width,
                (float) Setting<WindowSetting>::instance()->actual_height, 0.f, -16384.f, 16384.f);
        projection = scale(projection, glm::vec3 { 1, 1, 1 } * m_camera->scale_factor);
        SET_UNIFORMAT4P(shader, model_view_and_projection, projection * view);
    }

    if (m_terrain->heightmap() && m_terrain->heightmap()->ok()) {
        BIND_TEXTUREP(shader, heightmap, m_terrain->heightmap()->id(), 0);
    }
    SET_UNIFORM2P(shader, total_size, m_terrain->m_total_size);
    SET_UNIFORM2P(shader, chunk_size, glm::vec2(
            Constants::cellsPerChunkX * Constants::cellSize,
            Constants::cellsPerChunkY * Constants::cellSize));

    for (auto chunks : m_terrain->m_chunks) {
        for (auto chunk : chunks) {
            ASSERT(chunk->cached());
            glm::u8vec2 cacheidx = chunk->cacheData()->index;
            glm::vec2 cachepos = { (float) cacheidx.x / 8.0,
                    (float) cacheidx.y / 8.0 };

            SET_UNIFORM2P(shader, chunk_position, chunk->position());
            SET_UNIFORM2P(shader, cache_position, cachepos);
            BIND_TEXTUREP(shader, texcache_diffuse,
                    chunk->cacheData()->cache.lock()->textures()[DuneTextureType::DColor]->id(), 1);
            glDrawArrays(GL_TRIANGLES, 0, m_terrain->vertexBuffer()->countElements());
        }
    }
}

DuneTextureCache::DuneTextureCache() {
    for (size_t i = DuneTextureType::DColor; i < DuneTextureType::DEnd; i++) {
        std::string name = "DuneTextureCache_"; name += i;
        this->m_textures[i] = std::make_shared<Verdandi::TextureImage>(name);
        this->m_textures[i]->loadEmpty({ 4096, 4096 }, Verdandi::ImageChannelType::IRGB);
    }
}

void DuneTerrain::cacheChunk(std::shared_ptr<DuneChunk> chunk) {
    ASSERT(chunk);
    ASSERT(!chunk->cached());

    std::shared_ptr<DuneTextureCache> cache = m_caches[0];
    glm::u8vec2 pos;
    if (!cache->full()) {
        pos = cache->pickAnEntryWhenNotFull();
    } else {
        pos = cache->pickAndKickAnEntry()->index;
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
    data->index = idx;
    data->chunk = chunk;
    data->cache = shared_from_this();

    m_queue.push_front(data);
    m_caches[idx.x][idx.y] = data;
    m_cache_cache.insert({ chunk.get(), data });
    data->updateChunkWithSelf(data);

    return data;
}

std::shared_ptr<DuneTextureCacheData> DuneTextureCache::pickAndKickAnEntry() {
    std::shared_ptr<DuneTextureCacheData> ret = std::make_shared<DuneTextureCacheData>();
    if (empty()) {
        ret->index = pickAnEntryWhenNotFull();
    } else {
        ASSERT(!m_queue.empty());
        std::weak_ptr<DuneTextureCacheData> datap = m_queue.back();
        ASSERT(!datap.expired());
        {
            std::shared_ptr<DuneTextureCacheData> data = datap.lock();
            memcpy(ret.get(), data.get(), sizeof(DuneTextureCacheData));
        }
        {
            ASSERT(!ret->chunk.expired());
            DuneChunk *chunk = ret->chunk.lock().get();
            auto i = m_cache_cache.find(chunk);
            ASSERT(i != m_cache_cache.end() && !i->second.expired());
            m_cache_cache.erase(i);

            ASSERT(entryHold(ret->index));
            m_caches[ret->index.x][ret->index.y] = std::weak_ptr<DuneTextureCacheData>();
        }
        // if the only shared_ptr disappeared,
        //  the weak_ptr in DuneChunk would hopefully expire.
        log("Dune", L::Message) << "DuneTextureCache::pickAndKickAnEntry - kicking ("
            << ret->index.x << ", " << ret->index.y << ")" << rn;
        m_queue.pop_back();
    }
    return ret;
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
