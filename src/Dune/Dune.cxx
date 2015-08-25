//
// Created by secondwtq <lovejay-lovemusic@outlook.com> 2015/08/25.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of ISDCNext.
//
// We have always treaded the borderland.
//

#include "Dune.hxx"

#include "DuneCommon.hxx"
#include <vector>

namespace Howard {

namespace Verdandi {

}

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
        m_vert_buffer(Verdandi::VertexBufferSingle<VertFormatDuneTerrain>
        ::createWithVector(Verdandi::BufferUsage::UStatic, Helper::generateChunkVertex(
                { Constants::cellsPerChunkX, Constants::cellsPerChunkY }, Constants::cellSize))) {

    for (size_t i = 0; i < num_chunks.x; i++) {
        for (size_t j = 0; j < num_chunks.y; j++) {
            m_chunks[i].emplace_back(std::make_shared<DuneChunk>(this, HPixel(i * Constants::cellsPerChunkX *
                            Constants::cellSize, j * Constants::cellsPerChunkY * Constants::cellSize)));
        }
    }

}

DuneChunk::DuneChunk(DuneTerrain *parent, const HPixel& position) :
        m_parent(parent), m_position(position) {

}

}
}
