//
// Created by secondwtq <lovejay-lovemusic@outlook.com> 2015/05/24.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of ISDCNext.
//
// We have always treaded the borderland.
//

#ifndef HOWARD11_STANNUM_HXX
#define HOWARD11_STANNUM_HXX

#include <list>
#include <vector>
#include <glm/glm.hpp>

#include "Debug.hxx"

#include "Verdandi/GLCommon.hxx"
#include "Verdandi/GLFoundation.hxx"
#include "StannumShaderCache.hxx"

#include <stdlib.h>
#include <memory>

namespace Howard {
namespace Verdandi {
class VertexArray;
}
namespace Stannum {

class MappedVertexBufferBase {
    public:

    virtual ~MappedVertexBufferBase() { }

    virtual size_t start_point() = 0;
    virtual void *data_ptr() = 0;
    virtual size_t length_in_bytes() = 0;

    virtual void clear() = 0;
    virtual void upload() = 0;
};

template <typename T>
class MappedVertexBuffer;

class SharedVertexBuffer {
public:

    void init(size_t size);

    void clear() {
        for (auto map : m_mappings) {
            map->clear(); }
    }

    inline int id() {
        return this->m_buf_id; }

    void bind();
    void upload();

    template<typename T>
    MappedVertexBuffer<T> *map(size_t count);

private:

    int m_buf_id = -1;
    size_t m_size = 0;
    size_t mapped_end = 0;
    std::vector<MappedVertexBufferBase *> m_mappings;

};

// 150901: maybe we donot need it later
template <typename T>
class MappedVertexBuffer : public MappedVertexBufferBase {
public:

    MappedVertexBuffer(SharedVertexBuffer *parent, size_t count, size_t map_start) :
            m_parent(parent), m_start(map_start), m_count(count) { }

    size_t push(const T *src) {
        if (this->m_count > this->m_data.size()) {
            this->m_data.push_back(*src);
            return this->m_data.size()-1;
        } else {
            ASSERT_FOUNDATION(); }
        return 0;
    }

    void clear() override {
        this->m_data.clear(); }
    void bind() {
        parent()->bind(); }
    // TODO: ...
    void upload() {
        parent()->upload(); }

    SharedVertexBuffer *parent() {
        return m_parent; }

    inline size_t start_point() override {
        return m_start; }
    inline void *data_ptr() override {
        return (void *) m_data.data(); }
    inline size_t length_in_bytes() override {
        return m_count * sizeof(T); }

public:
    SharedVertexBuffer *m_parent = nullptr;
    size_t m_start = 0;
    size_t m_count = 0;
    std::vector<T> m_data;

};

enum CommandType {
    CDefault,
    CTest,
    CDuneTerrain
};

class StannumRenderer;

class StannumCommand {
public:

    virtual ~StannumCommand() { }
    virtual CommandType cmd_type() const { return CommandType::CDefault; }
    virtual void execute(StannumRenderer *renderer) = 0;

};

class StannumDispatchCommand {
public:

    virtual ~StannumDispatchCommand() { }
    virtual DispatchCommandType commandType() const {
        return DispatchCommandType::DDefault; }
    virtual void execute(StannumRenderer *renderer) = 0;

};

class RenderQueue {
public:

    ~RenderQueue() {
        // RenderQueue must be cleared
        ASSERT(commands.empty());
    }

    void push(StannumCommand *command) {
        commands.push_back(command); }

    inline void pushDispatchCommand(std::shared_ptr<StannumDispatchCommand> cmd) {
        dispatch_commands.push_back(cmd); }

    void clear() {
        for (auto cmd : commands) {
            delete cmd; }
        commands.clear();
    }

    std::list<StannumCommand *> commands;
    std::vector<std::shared_ptr<StannumDispatchCommand>> dispatch_commands;
};

struct DataSprite;

class StannumRenderer {
public:

    void init();
    void destroy();

    void render_dispatch(std::shared_ptr<RenderQueue> queue);

    inline ShaderCache *shaders() { return &this->m_shader_cache; }

    inline SHARED(Verdandi::VertexArray) vao() {
        return m_vao; }

private:
    friend class CommandSprite;
    friend class DispatchCommandSprite;

    void initializeVAO();

    std::vector<DataSprite *> m_sprites;
    SharedVertexBuffer m_shared_vb;
    ShaderCache m_shader_cache;
    SHARED(Verdandi::VertexArray) m_vao;
};

}
}

#endif // HOWARD11_STANNUM_HXX
