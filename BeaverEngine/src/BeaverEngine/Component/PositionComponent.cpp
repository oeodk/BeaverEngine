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

	void PositionComponent::resolve()
	{
		for (auto& child : owner().getChildren())
		{
			if (auto position = child->getComponent<PositionComponent>())
			{
				glm::vec3 relative_pos = position->getWorldPosition();
				position->setPosition(position_.x + relative_pos.x, position_.y + relative_pos.y, position_.z + relative_pos.z);
			}
		}
	}

	void PositionComponent::setPosition(float x, float y, float z)
	{
		for (auto& child : owner().getChildren())
		{
			if (auto position = child->getComponent<PositionComponent>())
			{
				glm::vec3 relative_pos = position->getRelativePosition();
				position->setPosition(x + relative_pos.x, y + relative_pos.y, z + relative_pos.z);
			}
		}
		position_.x = x;
		position_.y = y;
		position_.z = z;
	}
	
	void PositionComponent::move(float x, float y, float z)
	{
		for (auto& child : owner().getChildren())
		{
			if (auto position = child->getComponent<PositionComponent>())
			{
				position->move(x, y, z);
			}
		}
		position_.x += x;
		position_.y += y;
		position_.z += z;
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
		setPosition(parent_pos_comp->position_.x + x, parent_pos_comp->position_.y + y, parent_pos_comp->position_.z + z);
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