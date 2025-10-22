#include "BeaverEngine/Platform/Desktop/DesktopBufferImpl.h"
#include "BeaverEngine/Utils/Vertex.h"

namespace bv
{
	template<typename V>
	std::unique_ptr<VertexBufferImpl<V>> VertexBufferImpl<V>::create()
	{
		return std::make_unique<DesktopVertexBufferImpl<V>>();
	}

	std::unique_ptr<IndexBufferImpl> IndexBufferImpl::create()
	{
		return std::make_unique<DesktopIndexBufferImpl>();
	}
}
template class bv::DesktopVertexBufferImpl<bv::Vertex2D>;
template class bv::DesktopVertexBufferImpl<bv::Vertex3D>;
template class bv::VertexBufferImpl<bv::Vertex2D>;
template class bv::VertexBufferImpl<bv::Vertex3D>;