#pragma once
#include "BeaverEngine/Component/Component.h"
#include "BeaverEngine/Component/CollisionComponent.h"
#include "BeaverEngine/Utils/Array2D.h"

#include "BeaverEngine/Core/std.h"
namespace bv
{
	class LevelComponent
		: public CollisionComponent
	{
	public :
		LevelComponent(Entity& owner) : Component(owner) {}
		LevelComponent(const LevelComponent&) = default;
		LevelComponent& operator=(const LevelComponent&) = default;
		LevelComponent(LevelComponent&&) = default;
		LevelComponent& operator=(LevelComponent&&) = default;

		unsigned int getLevelWidth() const { return level_width_; }
		unsigned int getLevelHeight() const { return level_height_; }

	protected:
		Array2D<int> collision_grid;
		unsigned int level_height_{}, level_width_{};
	};
}
