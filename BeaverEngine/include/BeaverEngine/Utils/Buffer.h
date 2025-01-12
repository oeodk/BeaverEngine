#pragma once
#include "BeaverEngine/Core/std.h"
#include "BeaverEngine/Utils/BufferImpl.h"

namespace bv
{

	template<typename V>
	class VertexBuffer
	{
	private:
		std::unique_ptr<VertexBufferImpl<V>> impl_;
		std::unique_ptr<VertexBufferImpl<V>> create();
	public :
		VertexBuffer();
		VertexBuffer(const VertexBuffer&) = default;
		VertexBuffer(VertexBuffer&&) = default;
		VertexBuffer& operator=(const VertexBuffer&) = default;
		VertexBuffer& operator=(VertexBuffer&&) = default;

		~VertexBuffer() = default;

		void bind() const;
		void release() const;
		std::span<V> mapVertices(unsigned int start, unsigned int number_of_element);
		void setup();
	};

	class IndexBuffer
	{
	private:
		std::unique_ptr<IndexBufferImpl> impl_;
		std::unique_ptr<IndexBufferImpl> create();

	public:
		IndexBuffer();
		IndexBuffer(const IndexBuffer&) = default;
		IndexBuffer(IndexBuffer&&) = default;
		IndexBuffer& operator=(const IndexBuffer&) = default;
		IndexBuffer& operator=(IndexBuffer&&) = default;

		~IndexBuffer() = default;

		void bind() const;
		void release() const;

		void setup();

		unsigned int getIndexCount() const;
		std::span<unsigned int> mapIndices(unsigned int start, unsigned int number_of_element);

	};
}