#pragma once

#include "BeaverEngine/Component/PositionComponent.h"
#include "BeaverEngine/Component/SpriteComponent.h"
#include "BeaverEngine/Component/RectangleColliderComponent.h"
#include "BeaverEngine/Component/CircleColliderComponent.h"
#include "BeaverEngine/Component/LayerComponent.h"
#include "BeaverEngine/Component/View2DComponent.h"
#include "BeaverEngine/Component/WindowComponent.h"

namespace bv
{
	inline void registerBasicComponent()
	{
		Component::Register<CircleColliderComponent>();
		Component::Register<LayerComponent>();
		Component::Register<PositionComponent>();
		Component::Register<RectangleColliderComponent>();
		Component::Register<SpriteComponent>();
		Component::Register<View2DComponent>();
		Component::Register<WindowComponent>();
	}
}