#pragma once
#include "BeaverEngine/Platform/PlatformMacros.h"
#ifdef PLATFORM_DESKTOP

#include "BeaverEngine/Utils/BufferImpl.h"
#include GL_HEADER
#include "BeaverEngine/Core/Debug.h"
namespace bv
{
    template<typename V>
    class DesktopVertexBufferImpl : public VertexBufferImpl<V> {
    public:
        DesktopVertexBufferImpl() = default;

        ~DesktopVertexBufferImpl() {
            if (vbo_) glDeleteBuffers(1, &vbo_);
        }

        // Disable copy to avoid double deletion
        DesktopVertexBufferImpl(const DesktopVertexBufferImpl&) = delete;
        DesktopVertexBufferImpl& operator=(const DesktopVertexBufferImpl&) = delete;

        // Enable move
        DesktopVertexBufferImpl(DesktopVertexBufferImpl&& other) noexcept {
            vbo_ = std::exchange(other.vbo_, 0);
            created_ = std::exchange(other.created_, false);
            bufferCapacity_ = std::exchange(other.bufferCapacity_, 0);
        }

        DesktopVertexBufferImpl& operator=(DesktopVertexBufferImpl&& other) noexcept {
            if (this != &other) {
                if (vbo_) glDeleteBuffers(1, &vbo_);
                vbo_ = std::exchange(other.vbo_, 0);
                created_ = std::exchange(other.created_, false);
                bufferCapacity_ = std::exchange(other.bufferCapacity_, 0);
            }
            return *this;
        }

        void bind() const override {
            // Bind to binding index 0 (VAO must match)
            glBindVertexBuffer(0, vbo_, 0, sizeof(V));
        }

        void release() const override {
            // Optional; VAOs store binding state
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }

        void setup() override {
            size_t requiredSize = VertexBufferImpl<V>::vertices_.size();

            if (!created_) {
                glCreateBuffers(1, &vbo_);
                bufferCapacity_ = requiredSize;
                glNamedBufferData(vbo_, bufferCapacity_ * sizeof(V), nullptr, GL_DYNAMIC_DRAW);
                created_ = true;
            }

            // Grow buffer if needed
            if (requiredSize > bufferCapacity_) {
                bufferCapacity_ = requiredSize;
                glNamedBufferData(vbo_, bufferCapacity_ * sizeof(V), nullptr, GL_DYNAMIC_DRAW);
            }

            // Upload current vertex data
            if (requiredSize > 0) {
                glNamedBufferSubData(vbo_, 0, requiredSize * sizeof(V), VertexBufferImpl<V>::vertices_.data());
            }
        }

    private:
        GLuint vbo_ = 0;
        bool created_ = false;
        size_t bufferCapacity_ = 0;
    };
    class DesktopIndexBufferImpl : public IndexBufferImpl {
    public:
        DesktopIndexBufferImpl() = default;

        ~DesktopIndexBufferImpl() {
            if (ebo_) glDeleteBuffers(1, &ebo_);
        }

        DesktopIndexBufferImpl(const DesktopIndexBufferImpl&) = delete;
        DesktopIndexBufferImpl& operator=(const DesktopIndexBufferImpl&) = delete;

        DesktopIndexBufferImpl(DesktopIndexBufferImpl&& other) noexcept {
            ebo_ = std::exchange(other.ebo_, 0);
            created_ = std::exchange(other.created_, false);
        }

        DesktopIndexBufferImpl& operator=(DesktopIndexBufferImpl&& other) noexcept {
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
                bufferCapacity_ = indices_.size();
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