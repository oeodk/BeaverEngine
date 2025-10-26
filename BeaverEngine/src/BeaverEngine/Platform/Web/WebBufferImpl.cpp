#include "BeaverEngine/Platform/PlatformMacros.h"

#ifdef PLATFORM_WEB

#include "BeaverEngine/Platform/Web/WebBufferImpl.h"
#include "BeaverEngine/Utils/Vertex.h"



namespace bv
{
	template<typename V>
	std::unique_ptr<VertexBufferImpl<V>> VertexBufferImpl<V>::create()
	{
		return std::make_unique<WebVertexBufferImpl<V>>();
	}

	std::unique_ptr<IndexBufferImpl> IndexBufferImpl::create()
	{
		return std::make_unique<WebIndexBufferImpl>();
	}
}
template class bv::WebVertexBufferImpl<bv::Vertex2D>;
template class bv::WebVertexBufferImpl<bv::Vertex3D>;
template class bv::VertexBufferImpl<bv::Vertex2D>;
template class bv::VertexBufferImpl<bv::Vertex3D>;

#endif