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
#include "Verdandi/GLVertexArray.hxx"
#include "StannumShaderCache.hxx"
#include "StannumTexture.hxx"

#include <stdlib.h>
#include <memory>

namespace Howard {
namespace Stannum {

class MappedVertexBufferBase {
    public:

    virtual ~MappedVertexBufferBase() { }
    virtual void clear() = 0;
    virtual void upload() = 0;
};

template <typename T>
class MappedVertexBuffer;

class SharedVertexBuffer {

    public:

    void init(size_t size) {
        VGLIDX buf_t = 0;
        glGenBuffers(1, &(buf_t));
        this->m_buf_id = buf_t;
        glBindBuffer(GL_ARRAY_BUFFER, this->m_buf_id);
        char *data_t = (char *) malloc(size * 2);
        glBufferData(GL_ARRAY_BUFFER, size * 2, data_t, GL_DYNAMIC_DRAW);
        free(data_t);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        this->m_size = size;
        this->mapped_end = 0;
    }

    void clear() {
        for (auto map : m_mappings) {
            map->clear(); }
    }

    inline int id() { return this->m_buf_id; }

    void upload() {
        glBindBuffer(GL_ARRAY_BUFFER, this->m_buf_id);
        for (auto map : m_mappings) {
            map->upload(); }
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    template<typename T>
    MappedVertexBuffer<T> *map(size_t count);

    private:

    int m_buf_id = -1;
    size_t m_size = 0;
    size_t mapped_end = 0;
    std::vector<MappedVertexBufferBase *> m_mappings;

};

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

    void upload(size_t start, size_t count) {
        glBindBuffer(GL_ARRAY_BUFFER, this->m_parent->id());
        glBufferSubData(GL_ARRAY_BUFFER, this->m_start + sizeof(T) * start, sizeof(T) * count,
                        (void *) ((char *) m_data.data() + sizeof(T) * start));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void clear() override {
        this->m_data.clear(); }

    void upload() override {
        glBufferSubData(GL_ARRAY_BUFFER, this->m_start, sizeof(T) * m_data.size(), (void *) m_data.data()); }

    void bind() {
        glBindBuffer(GL_ARRAY_BUFFER, this->m_parent->id()); }

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

enum DispatchCommandType {
    DDefault,
    DSpriteDispatch,
    DDuneTerrain
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

    void render_dispatch(std::shared_ptr<RenderQueue> queue) {
        ASSERT(queue != nullptr);

        Verdandi::clear_depth();
        glClear(GL_COLOR_BUFFER_BIT);
        for (auto cmd : queue->commands) {
            cmd->execute(this); }
        for (auto cmd : queue->dispatch_commands) {
            cmd->execute(this); }
        queue->clear();
    }

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
