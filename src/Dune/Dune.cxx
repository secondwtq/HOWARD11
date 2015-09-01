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

#include "DuneTextureCache.hxx"
#include "DuneCommon.hxx"

#include "Verdandi/GLCommon.hxx"
#include "Verdandi/GLVertexBuffer.hxx"
#include "Verdandi/GLVertexArray.hxx"
#include "FSM/FSM.hxx"

#include "Guardian/Guardian.hxx"

#include <vector>

namespace Howard {
namespace Dune {

using namespace Verdandi;
using namespace FSMHelper;

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
        m_vert_buffer(VertexBufferSingle<VertFormatDuneTerrain>
        ::createWithVector(BufferUsage::UStatic, Helper::generateChunkVertex(
                { Constants::cellsPerChunkX * Constants::gridPerCellX,
                        Constants::cellsPerChunkY * Constants::gridPerCellY },
                Constants::gridSize))),
        m_renderer(renderer) {

    ASSERT(renderer);

    for (size_t i = 0; i < num_chunks.x; i++) {
        for (size_t j = 0; j < num_chunks.y; j++) {
            m_chunks[i].emplace_back(std::make_shared<DuneChunk>(this)); }
    }
    this->setOrigin({ });

    m_caches.emplace_back(std::make_shared<DuneTextureCache>(renderer));
    initializeVAO();
}

void DuneTerrain::setOrigin(const HAnyCoord& origin) {
    m_origin = origin;
    for (size_t i = 0; i < m_num_chunks.x; i++) {
        for (size_t j = 0; j < m_num_chunks.y; j++) {
            m_chunks[i][j]->setPosition({ origin.x + i * sizePerChunk2D().x,
                    origin.y + j * sizePerChunk2D().y, origin.z });
        }
    }
}

void DuneChunk::setPosition(const HAnyCoord& position) {
    m_position = position; }

void DuneTerrain::initializeVAO() {
    VertexArrayScope scope(m_vao);
    VertexBufferScope<VertexBufferSingle<VertFormatDuneTerrain>>
            buf(vertexBuffer());
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
            sizeof(VertFormatDuneTerrain), nullptr);
}

DuneChunk::DuneChunk(DuneTerrain *parent, const HAnyCoord& position) :
        m_parent(parent), m_position(position) { ASSERT(parent); }

void DuneChunk::cacheMarkUsed() {
    ASSERT(cached());
    cacheData()->cache.lock()->markCacheEntryUsed(cacheData());
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

void DuneChunk::updateCachedTexture() {
    ASSERT(cached());
    SHARED(DuneTextureCacheData) data = m_cache_data.lock();
    SHARED(Guardian::GuardianElementTerrainLayersGroup) element =
            std::make_shared<Guardian::GuardianElementTerrainLayersGroup>(data->index());
    for (auto layer : layers()) {
        element->addLayer(layer); }
//    if (element->numberOfLayers()) {
        data->cache.lock()->canvas()->paintBlock([ &element ](Guardian::GuardianCanvas *canvas) {
            element->paint(canvas); });
//    }
}

void DuneTerrain::setHeightmap(SHARED(TextureImage) heightmap) {
    m_heightmap = heightmap;

    if (m_heightmap) {
        m_scaled_height = std::make_shared<Image>("terrain_scaledheight");
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
            (float) Constants::heightSpanMax / 256.0 };
}

}
}
