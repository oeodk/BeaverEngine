#include "BeaverEngine/Utils/Buffer.h"
#include "BeaverEngine/Utils/BufferImpl.h"
#include "BeaverEngine/Utils/Vertex.h"
#include "BeaverEngine/Core/Renderer.h"
namespace bv
{
	template<typename V>
	std::unique_ptr<VertexBufferImpl<V>> VertexBuffer<V>::create()
	{
		return VertexBufferImpl<V>::create();
	}

	template<typename V>
	VertexBuffer<V>::VertexBuffer()
		: impl_(create())
	{
	}

	template<typename V>
    void VertexBuffer<V>::bind() const
	{
		impl_->bind();
	}
	template<typename V>
	void VertexBuffer<V>::release() const
	{
		impl_->release();
	}

	template<typename V>
	std::span<V> VertexBuffer<V>::mapVertices(unsigned int start, unsigned int number_of_element)
	{
		return impl_->mapVertices(start, number_of_element);
	}

	template<typename V>
	void VertexBuffer<V>::setup()
	{
		impl_->setup();
	}


	std::unique_ptr<IndexBufferImpl> IndexBuffer::create()
	{
		return IndexBufferImpl::create();
	}

	IndexBuffer::IndexBuffer()
		: impl_(create())
	{
	}

	void IndexBuffer::bind() const
	{
		impl_->bind();
	}

	void IndexBuffer::release() const
	{
		impl_->release();
	}

	void IndexBuffer::setup()
	{
		impl_->setup();
	}

	unsigned int IndexBuffer::getIndexCount() const
	{
		return impl_->getIndexCount();
	}

	std::span<unsigned int> IndexBuffer::mapIndices(unsigned int start, unsigned int number_of_element)
	{
		return impl_->mapIndices(start, number_of_element);
	}
}

template class bv::VertexBuffer<bv::Vertex2D>;
template class bv::VertexBuffer<bv::Vertex3D>;
template class bv::VertexBufferImpl<bv::Vertex2D>;
template class bv::VertexBufferImpl<bv::Vertex3D>;