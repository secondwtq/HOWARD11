//
// Created by secondwtq <lovejay-lovemusic@outlook.com> 2015/09/01.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of ISDCNext.
//
// We have always treaded the borderland.
//

#ifndef HOWARD11_STANNUMTEXTUREATLAS_HXX
#define HOWARD11_STANNUMTEXTUREATLAS_HXX

#include "Common.hxx"
#include "StannumTexture.hxx"

#include <string>
#include <vector>

namespace Howard {
namespace Verdandi {

struct TextureAtlasData {
    HPixel size;
    HPixel offset;
    HScale texCoordStart;
    HScale texCoordEnd;
};

class TextureAtlas : public Asset {
public:
    TextureAtlas(const std::string& name, const SHARED(TextureImage) parent);

    VGLIDX id() const {
        return m_parent->id(); }
    inline const char *asset_type() const override {
        return TextureAtlas::m_asset_type; }
    static constexpr const char m_asset_type[] = "TextureImage";

    static TextureAtlasData createDataWithAtlasSize(const TextureAtlas& atlas,
            const HPixel& position, const HPixel& size, const HPixel& offset) {
        HPixel psize = atlas.m_parent->size;

        TextureAtlasData ret;
        ret.size = size;
        ret.offset = offset;
        ret.texCoordStart = position / psize;
        ret.texCoordEnd = (position + size) / psize;

        return ret;
    }

    inline size_t numberOfGroups() const {
        return m_data.size(); }

    inline size_t addGroup() {
        m_data.emplace_back();
        return numberOfGroups();
    }

    inline size_t numberOfDataInGroup(size_t group_idx) const {
        if (group_idx < numberOfGroups()) {
            return m_data[group_idx].size();
        } else { return 0; }
    }

    inline size_t addData(size_t group_idx, const TextureAtlasData& data) {
        ASSERT(group_idx < numberOfGroups());
        m_data[group_idx].emplace_back(data);
        return numberOfDataInGroup(group_idx);
    }

    inline TextureAtlasData data(size_t group_idx, size_t id) {
        if (group_idx < numberOfGroups() &&
                id < numberOfDataInGroup(group_idx)) {
            return m_data[group_idx][id];
        } else {
            // safe? SAFE?!
            //  if it's const TextureAtlasData& ?
            return { };
        }
    }

private:

    std::vector<std::vector<TextureAtlasData>> m_data;
    const SHARED(TextureImage) m_parent;
};

}
}

#endif // HOWARD11_STANNUMTEXTUREATLAS_HXX
