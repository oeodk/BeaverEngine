#pragma once
#include "BeaverEngine/Core/std.h"

namespace bv
{
	template<typename V>
	class VertexBufferImpl
	{
	public :
		VertexBufferImpl() = default;
		VertexBufferImpl(const VertexBufferImpl&) = default;
		VertexBufferImpl(VertexBufferImpl&&) = default;
		VertexBufferImpl& operator=(const VertexBufferImpl&) = default;
		VertexBufferImpl& operator=(VertexBufferImpl&&) = default;

		virtual ~VertexBufferImpl() = default;

		static std::unique_ptr<VertexBufferImpl> create();

		virtual void bind() const = 0;
		virtual void release() const = 0;
		virtual void setup() = 0;

		std::span<V> mapVertices(unsigned int start, unsigned int number_of_element)
		{
			if (vertices_.size() < start + number_of_element)
			{
				vertices_.resize(start + number_of_element);
			}
			return std::span<V>(vertices_.data() + start, number_of_element);
		}
	protected:
		std::vector<V>& getVertices() { return vertices_; }
		std::vector<V> vertices_;
	};

	class IndexBufferImpl
	{
	public:
		IndexBufferImpl() = default;
		IndexBufferImpl(const IndexBufferImpl&) = default;
		IndexBufferImpl(IndexBufferImpl&&) = default;
		IndexBufferImpl& operator=(const IndexBufferImpl&) = default;
		IndexBufferImpl& operator=(IndexBufferImpl&&) = default;

		virtual ~IndexBufferImpl() = default;

		static std::unique_ptr<IndexBufferImpl> create();

		virtual void bind() const = 0;
		virtual void release() const = 0;
		virtual void setup() = 0;

		unsigned int getIndexCount() const { return indices_.size(); }

		std::span<unsigned int> mapIndices(unsigned int start, unsigned int number_of_element)
		{
			if (indices_.size() < start + number_of_element)
			{
				indices_.resize(start + number_of_element);
			}
			return std::span<unsigned int>(indices_.data() + start, number_of_element);
		}
	protected:
		std::vector<unsigned int>& getIndices() { return indices_; }
		std::vector<unsigned int> indices_;
	};
}

