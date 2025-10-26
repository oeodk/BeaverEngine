#pragma once
#include "BeaverEngine/Platform/PlatformMacros.h"

#ifdef PLATFORM_WEB

#include "BeaverEngine/Utils/BufferImpl.h"
#include "BeaverEngine/Utils/Vertex.h"
#include GL_HEADER
#include "BeaverEngine/Core/Debug.h"
namespace bv
{
    template<typename V>
    class WebVertexBufferImpl : public VertexBufferImpl<V> {
    public:
        WebVertexBufferImpl() = default;

        ~WebVertexBufferImpl() {
            if (vbo_) glDeleteBuffers(1, &vbo_);
        }

        // Disable copy to avoid double deletion
        WebVertexBufferImpl(const WebVertexBufferImpl&) = delete;
        WebVertexBufferImpl& operator=(const WebVertexBufferImpl&) = delete;

        // Enable move
        WebVertexBufferImpl(WebVertexBufferImpl&& other) noexcept {
            vbo_ = std::exchange(other.vbo_, 0);
            created_ = std::exchange(other.created_, false);
            bufferCapacity_ = std::exchange(other.bufferCapacity_, 0);
        }

        WebVertexBufferImpl& operator=(WebVertexBufferImpl&& other) noexcept {
            if (this != &other) {
                if (vbo_) glDeleteBuffers(1, &vbo_);
                vbo_ = std::exchange(other.vbo_, 0);
                created_ = std::exchange(other.created_, false);
                bufferCapacity_ = std::exchange(other.bufferCapacity_, 0);
            }
            return *this;
        }

        void bind() const override {
            glBindVertexArray(vao_);
        }

        void release() const override {
            glBindVertexArray(0);
        }

        void setup() override {
            size_t requiredSize = VertexBufferImpl<V>::vertices_.size();

            if (!created_) {
                glGenBuffers(1, &vbo_);
                bufferCapacity_ = requiredSize;

                glBindBuffer(GL_ARRAY_BUFFER, vbo_);
                glBufferData(GL_ARRAY_BUFFER, bufferCapacity_ * sizeof(V), nullptr, GL_DYNAMIC_DRAW);

                created_ = true;
            }

            // Grow buffer if needed
            if (requiredSize > bufferCapacity_) {
                bufferCapacity_ = static_cast<size_t>(requiredSize * 1.5f);;

                glBindBuffer(GL_ARRAY_BUFFER, vbo_);
                glBufferData(GL_ARRAY_BUFFER, bufferCapacity_ * sizeof(V), nullptr, GL_DYNAMIC_DRAW);
            }

            // Upload current vertex data
            if (requiredSize > 0) {
                glBindBuffer(GL_ARRAY_BUFFER, vbo_);
                glBufferSubData(GL_ARRAY_BUFFER, 0, requiredSize * sizeof(V), VertexBufferImpl<V>::vertices_.data());

            }

            if (init_vao_)
            {
                glGenVertexArrays(1, &vao_);
                glBindVertexArray(vao_);

                glBindBuffer(GL_ARRAY_BUFFER, vbo_);
                if(sizeof(V) == sizeof(Vertex2D))
                {

                    // Attribute array for the coordinates of the vertices
                    glEnableVertexAttribArray(0);
                    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (const void*)offsetof(Vertex2D, position));
                    // Attribute array for the texture coords of the vertices
                    glEnableVertexAttribArray(1);
                    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (const void*)offsetof(Vertex2D, texture_coords));
                    // Attribute array for the color of the vertices
                    glEnableVertexAttribArray(2);
                    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (const void*)offsetof(Vertex2D, color));

                }
                if (sizeof(V) == sizeof(Vertex3D))
                {
                    // Attribute array for the coordinates of the vertices
                    glEnableVertexAttribArray(0);
                    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (const void*)offsetof(Vertex3D, position));
                    // Attribute array for the texture coords of the vertices
                    glEnableVertexAttribArray(1);
                    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (const void*)offsetof(Vertex3D, normal));
                    // Attribute array for the color of the vertices
                    glEnableVertexAttribArray(2);
                    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (const void*)offsetof(Vertex3D, texture_coords));

                }
                // Unbind the VAO (optional)
                glBindVertexArray(0);
                glBindBuffer(GL_ARRAY_BUFFER, 0);
                init_vao_ = false;
            }
        }

    private:
        GLuint vbo_ = 0;
        bool created_ = false;
        bool init_vao_ = true;
        size_t bufferCapacity_ = 0;

        GLuint vao_ = 0;
    };
    class WebIndexBufferImpl : public IndexBufferImpl {
    public:
        WebIndexBufferImpl() = default;

        ~WebIndexBufferImpl() {
            if (ebo_) glDeleteBuffers(1, &ebo_);
        }

        WebIndexBufferImpl(const WebIndexBufferImpl&) = delete;
        WebIndexBufferImpl& operator=(const WebIndexBufferImpl&) = delete;

        WebIndexBufferImpl(WebIndexBufferImpl&& other) noexcept {
            ebo_ = std::exchange(other.ebo_, 0);
            created_ = std::exchange(other.created_, false);
        }

        WebIndexBufferImpl& operator=(WebIndexBufferImpl&& other) noexcept {
            if (this != &other) {
                if (ebo_) glDeleteBuffers(1, &ebo_);
                ebo_ = std::exchange(other.ebo_, 0);
                created_ = std::exchange(other.created_, false);
            }
            return *this;
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
            if (!created_) {
                glGenBuffers(1, &ebo_);
                created_ = true;
            }
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
            if (bufferCapacity_ < indices_.size()) {
                bufferCapacity_ = static_cast<size_t>(indices_.size() * 1.5f);
                
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufferCapacity_ * sizeof(unsigned int), indices_.data(), GL_DYNAMIC_DRAW);
            }
            else {
                glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indices_.size() * sizeof(unsigned int), indices_.data());
            }
        }

    private:
        GLuint ebo_ = 0;
        bool created_ = false;
        size_t bufferCapacity_ = 0;
    };
}

#endif