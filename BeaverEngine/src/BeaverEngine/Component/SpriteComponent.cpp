#include "BeaverEngine/Component/SpriteComponent.h"
#include "BeaverEngine/Component/PositionComponent.h"

#include "BeaverEngine/Core/Entity.h"
#include "BeaverEngine/Core/Scene.h"
#include "BeaverEngine/Core/GlobalConstants.h"

#include "BeaverEngine/Component/LayerComponent.h"

#include "BeaverEngine/Utils/Window.h"

#include <glad/glad.h>
#include <glm/gtc/quaternion.hpp>

#include "BeaverEngine/System/InputSystem.h"
#include <iostream>
namespace bv
{
	SpriteComponent::~SpriteComponent()
	{
		if(layer_ != nullptr)
		{
			layer_->removeSprite(this);
		}
	}

	void SpriteComponent::setup(const ComponentDescription& init_value)
	{
		for (auto& value : init_value.parameters)
		{
			switch (string_to_init_enum_map_.at(value.first))
			{
			case bv::SpriteComponent::SIZE:
				initSize(value.second);
				fixed_size_ = true;
				break;
			case bv::SpriteComponent::SCALE:
				initScale(value.second);
				break;
			case bv::SpriteComponent::OFFSET:
				initOffset(value.second);
				break;		
			case bv::SpriteComponent::COLOR:
				initColor(value.second);
				break;
			case bv::SpriteComponent::RENDER_RECT:
				init_render_rect_ = false;
				initRenderRect(value.second);
				break;
			case bv::SpriteComponent::ROTATION_ANGLE:
				initAngle(value.second);
				break;
			case bv::SpriteComponent::ANIMATION:
				initAnimation(value.second);
				break;
			case bv::SpriteComponent::LAYER:
				initLayer(value.second);
				break;
			}
		}
	}

	void SpriteComponent::resolve()
	{
		auto layer_entity = Scene::findObject(layer_name);
		layer_ = layer_entity->getComponent<LayerComponent>();

		if (!fixed_size_)
		{
			auto texture = layer_->getTexture().lock();
			size_.x = texture->getWidth();
			size_.y = texture->getHeight();
		}
		if (init_render_rect_)
		{
			auto texture = layer_->getTexture().lock();
			render_rect_.size.x = texture->getWidth();
			render_rect_.size.y = texture->getHeight();
		}

		refreshPoint();
		setRenderRectangle(render_rect_);
		updateMesh();

		layer_->addSprite(this);
	}

	void SpriteComponent::updateLogic(const Timing& timing)
	{
		updateAnimation(timing);
		updateMesh();
	}

	void SpriteComponent::updateAnimation(const Timing& timing)
	{
		animation_dt_ += timing.dt_.count();
		const auto texture = layer_->getTexture().lock();
		setRenderRectangle(FloatRect(texture->getSpriteCoordinate(animation_name_, animation_dt_), texture->getSpriteSize(animation_name_)));
	}

	void SpriteComponent::updateMesh()
	{
		const PositionComponent* position = owner().getComponent<PositionComponent>();
		
		vertices_[0] = { position->getWorldPosition() + points_[0], texture_coords_[0] };
		vertices_[1] = { position->getWorldPosition() + points_[1], texture_coords_[1] };
		vertices_[2] = { position->getWorldPosition() + points_[2], texture_coords_[2] };
		vertices_[3] = { position->getWorldPosition() + points_[3], texture_coords_[3] };
	}
	void SpriteComponent::setRenderRectangle(const FloatRect& render_rect)
	{
		const auto texture = layer_->getTexture().lock();
		const unsigned int texture_width = texture->getWidth();
		const unsigned int texture_height = texture->getHeight();
		render_rect_ = render_rect;
		texture_coords_[0].x = render_rect_.pos.x / texture_width;
		texture_coords_[1].y = 1 - render_rect_.pos.y / texture_height;

		texture_coords_[1].x = render_rect_.pos.x / texture_width;
		texture_coords_[0].y = 1 - (render_rect_.pos.y + render_rect_.size.y) / texture_height;

		texture_coords_[2].x = (render_rect_.pos.x + render_rect_.size.x) / texture_width;
		texture_coords_[3].y = 1 - (render_rect_.pos.y + render_rect_.size.y) / texture_height;

		texture_coords_[3].x = (render_rect_.pos.x + render_rect_.size.x) / texture_width;
		texture_coords_[2].y = 1 - render_rect_.pos.y / texture_height;

		if (!fixed_size_)
		{
			setSize(render_rect_.size);
		}
	}

	void SpriteComponent::refreshPoint()
	{
		points_[0].x = -scale_.x * (size_.x / 2.f) + offset_.x;
		points_[0].y = -scale_.y * (size_.y / 2.f) + offset_.y;
		
		points_[1].x = -scale_.x * (size_.x / 2.f) + offset_.x;
		points_[1].y =  scale_.y * (size_.y / 2.f) + offset_.y;
		
		points_[2].x =  scale_.x * (size_.x / 2.f) + offset_.x;
		points_[2].y =  scale_.y * (size_.y / 2.f) + offset_.y;

		points_[3].x =  scale_.x * (size_.x / 2.f) + offset_.x;
		points_[3].y = -scale_.y * (size_.y / 2.f) + offset_.y;

		if(rotation_angle_ != 0)
		{
			glm::vec3 axis(0.0f, 0.0f, 1.0f);
			float angle = glm::radians(rotation_angle_);

			glm::quat rotation = glm::angleAxis(angle, glm::normalize(axis));
			for (auto& point : points_)
			{
				point = rotation * point;
			}
		}
	}

	void SpriteComponent::initColor(const Description& value)
	{
		color_.r = value[0].as<float>();
		color_.g = value[1].as<float>();
		color_.b = value[2].as<float>();
		color_.a = value[3].as<float>();
	}
	void SpriteComponent::initSize(const Description & value)
	{
		size_.x = value[0].as<float>();
		size_.y = value[1].as<float>();
	}

	void SpriteComponent::initOffset(const Description& value)
	{
		offset_.x = value[0].as<float>();
		offset_.y = value[1].as<float>();
	}

	void SpriteComponent::initScale(const Description& value)
	{
		scale_.x = value[0].as<float>();
		scale_.y = value[1].as<float>();
	}

	void SpriteComponent::initRenderRect(const Description& value)
	{
		render_rect_.pos.x = value[0].as<float>();
		render_rect_.pos.y = value[1].as<float>();
		render_rect_.size.x = value[2].as<float>();
		render_rect_.size.y = value[3].as<float>();
	}

	void SpriteComponent::initAngle(const Description& value)
	{
		rotation_angle_ = value.as<float>();
	}

	void SpriteComponent::initLayer(const Description& value)
	{
		layer_name = value.as<std::string>();
	}

	void SpriteComponent::initAnimation(const Description& value)
	{
		animation_name_ = value.as<std::string>();
	}

	bool SpriteComponent::compareSpritesPosition(const SpriteComponent* c1, const SpriteComponent* c2)
	{
		const glm::vec3& s1_pos = c1->owner().getComponent<PositionComponent>()->getWorldPosition();
		const glm::vec3& s2_pos = c2->owner().getComponent<PositionComponent>()->getWorldPosition();
		if (s1_pos.y != s2_pos.y)
			return s1_pos.y < s2_pos.y;
		return s1_pos.x < s2_pos.x;
	}
}