#pragma once

#include "BeaverEngine/Component/PositionComponent.h"
#include "BeaverEngine/Component/SpriteComponent.h"
#include "BeaverEngine/Component/WindowManagerComponent.h"
#include "BeaverEngine/Component/ViewManagerComponent.h"
#include "BeaverEngine/Component/TextureManagerComponent.h"
#include "BeaverEngine/Component/InputManagerComponent.h"
#include "BeaverEngine/Component/RectangleColliderComponent.h"
#include "BeaverEngine/Component/CircleColliderComponent.h"

namespace bv
{
	inline void registerBasicComponent()
	{
		Component::Register<CircleColliderComponent>();
		Component::Register<InputManagerComponent>();
		Component::Register<PositionComponent>();
		Component::Register<RectangleColliderComponent>();
		Component::Register<SpriteComponent>();
		Component::Register<TextureManagerComponent>();
		Component::Register<ViewManagerComponent>();
		Component::Register<WindowManagerComponent>();
	}
}