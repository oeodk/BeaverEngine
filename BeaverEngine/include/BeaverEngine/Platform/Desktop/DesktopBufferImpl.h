#pragma once
#include "BeaverEngine/Utils/BufferImpl.h"
#include <glad/glad.h>
#include "BeaverEngine/Core/Debug.h"
namespace bv
{
    template<typename V>
    class DesktopVertexBufferImpl
        : public VertexBufferImpl<V>
    {
    public:
        DesktopVertexBufferImpl() = default;

        ~DesktopVertexBufferImpl()
        {
            glDeleteBuffers(1, &vbo_);
        }

        void bind() const override
        {
            glBindVertexBuffer(0, vbo_, 0, sizeof(V));
            glNamedBufferSubData(vbo_, 0, VertexBufferImpl<V>::vertices_.size() * sizeof(V), VertexBufferImpl<V>::vertices_.data());

            //glBindBuffer(GL_ARRAY_BUFFER, vbo_);
            //glBufferSubData(GL_ARRAY_BUFFER, 0, VertexBufferImpl<V>::vertices_.size() * sizeof(V), VertexBufferImpl<V>::vertices_.data());
        }
        void release() const override
        {
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }

        void setup() override
        {
            if (created_)
            {
                glDeleteBuffers(1, &vbo_);
            }
            glCreateBuffers(1, &vbo_);
            glNamedBufferData(vbo_, VertexBufferImpl<V>::vertices_.size() * sizeof(V), VertexBufferImpl<V>::vertices_.data(), GL_DYNAMIC_DRAW);
            
            created_ = true;
            //glGenBuffers(1, &vbo_);
            //glBindBuffer(GL_ARRAY_BUFFER, vbo_);
            //glBufferData(GL_ARRAY_BUFFER, , nullptr, GL_DYNAMIC_DRAW);
        }
    private:
        bool created_{};
        GLuint vbo_;
    };

    class DesktopIndexBufferImpl
        : public IndexBufferImpl    
    {
    public:
        DesktopIndexBufferImpl() = default;

        ~DesktopIndexBufferImpl()
        {
            glDeleteBuffers(1, &ebo_);
        }

        void bind() const override
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
        }

        void release() const override
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }

        void setup() override
        {
            glGenBuffers(1, &ebo_);        
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(unsigned int), indices_.data(), GL_STATIC_DRAW);
        }
    private:
        GLuint ebo_;
    };
}

