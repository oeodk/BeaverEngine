#include "BeaverEngine/Component/PositionComponent.h"
#include "BeaverEngine/Core/Entity.h"

namespace bv
{
	void PositionComponent::setup(const ComponentDescription& init_value)
	{
		position_.x = init_value.parameters.at("x").as<float>();
		position_.y = init_value.parameters.at("y").as<float>();
		position_.z = init_value.parameters.at("z").as<float>();
	}

	void PositionComponent::setRelativePosition(float x, float y, float z)
	{
        
		EntityRef parent = owner().getParent();
		if (parent == nullptr)
		{
			setPosition(x, y, z);
			return;
		}
		PositionComponent* parent_pos_comp = parent->getComponent<PositionComponent>();
		if (parent_pos_comp == nullptr)
		{
			setPosition(x, y, z);
			return;
		}

		position_.x = parent_pos_comp->position_.x + x;
		position_.y = parent_pos_comp->position_.y + y;
		position_.z = parent_pos_comp->position_.z + z;
   
	}
	glm::vec3 PositionComponent::getRelativePosition() const
	{
		EntityRef parent = owner().getParent();
		if (parent == nullptr)
		{
			return position_;
		}
		PositionComponent* parent_pos_comp = parent->getComponent<PositionComponent>();
		if (parent_pos_comp == nullptr)
		{
			return position_;
		}
		return position_ - parent_pos_comp->position_;
	}
}