#include "BeaverEngine/Utils/Texture2D.h"
#include "BeaverEngine/Component/TextureManagerComponent.h"
#include "BeaverEngine/Core/Scene.h"
#include "BeaverEngine/Core/Entity.h"
namespace bv
{
	Texture2D::~Texture2D()
	{
		ManagerComponent::getManager<TextureManagerComponent>()->texture2dRemoved(getPath());
	}
}