#include "BeaverEngine/Component/SpriteComponent.h"
#include "BeaverEngine/Component/PositionComponent.h"

#include "BeaverEngine/Core/Entity.h"
#include "BeaverEngine/Core/Renderer.h"
#include "BeaverEngine/Core/Scene.h"
#include "BeaverEngine/Core/GlobalConstants.h"

#include "BeaverEngine/Component/ViewManagerComponent.h"
#include "BeaverEngine/Component/WindowManagerComponent.h"
#include "BeaverEngine/Component/TextureManagerComponent.h"
#include "BeaverEngine/System/EntitySystem.h"

#include "BeaverEngine/Utils/Window.h"

#include <glad/glad.h>
#include <glm/gtc/quaternion.hpp>

#include "BeaverEngine/Component/InputManagerComponent.h"
#include <iostream>
namespace bv
{
	void SpriteComponent::setup(const ComponentDescription& init_value)
	{
		std::weak_ptr<Window> main_window = ManagerComponent::getManager<WindowManagerComponent>()->getMainWindow();;
		main_window.lock()->makeCurrent();
		bool init_window = true;
		bool init_view = true;
		bool init_render_rect = true;
		for (auto& value : init_value.parameters)
		{
			switch (string_to_init_enum_map_.at(value.first))
			{
			case bv::SpriteComponent::SIZE:
				initSize(value.second);
				fixed_size_ = true;
				break;
			case bv::SpriteComponent::WINDOW:
				initWindow(value.second);
				init_window = false;
				break;
			case bv::SpriteComponent::SCALE:
				initScale(value.second);
				break;
			case bv::SpriteComponent::VIEW:
				initView(value.second);
				init_view = false;
				break;
			case bv::SpriteComponent::OFFSET:
				initOffset(value.second);
				break;
			case bv::SpriteComponent::TEXTURE:
			{
				bool interpolate = false;
				
				if(init_value.parameters.contains("interpolate_texture") && init_value.parameters.at("interpolate_texture").as<bool>() == true) 
				{
					interpolate = true;
				}
				initTexture(value.second, interpolate);
			}
				break;
			case bv::SpriteComponent::COLOR:
				initColor(value.second);
				break;
			case bv::SpriteComponent::RENDER_RECT:
				init_render_rect = false;
				initRenderRect(value.second);
				break;
			case bv::SpriteComponent::ROTATION_ANGLE:
				initAngle(value.second);
				break;
			case bv::SpriteComponent::ANIMATION:
				initAnimation(value.second);
				break;
			}
		}
		if (init_window)
		{
			window_to_render_ = ManagerComponent::getManager<WindowManagerComponent>()->getMainWindow();
		}
		if (init_view)
		{
			view_to_render_ = ManagerComponent::getManager<ViewManagerComponent>()->getMainView();
		}
		if(!fixed_size_)
		{
			size_.x = texture_->getWidth();
			size_.y = texture_->getHeight();
		}
		if (init_render_rect)
		{
			render_rect_.size.x = texture_->getWidth();
			render_rect_.size.y = texture_->getHeight();
		}
		refreshPoint();
		setRenderRectangle(render_rect_);

		auto mapped_indices = index_buffer_.mapIndices(0, 6);
		std::copy(INDICES.begin(), INDICES.end(), mapped_indices.begin());
		index_buffer_.setup();

		updateMesh();
		vertex_buffer_.setup();

		//texture stuff
	}

	void SpriteComponent::updateLogic(const Timing& timing)
	{
		//PositionComponent* position = owner().getComponent<PositionComponent>();
		//glm::vec2 pos = position->getWorldPosition();
		//if (Input::isKeyPressed(Key::UP))
		//{
		//	pos.y += 10;
		//}
		//if (Input::isKeyPressed(Key::DOWN))
		//{
		//	pos.y -= 10;
		//}
		//if (Input::isKeyPressed(Key::RIGHT))
		//{
		//	pos.x += 10;
		//}
		//if (Input::isKeyPressed(Key::LEFT))
		//{
		//	pos.x -= 10;
		//}
		//position->setPosition(pos);

			auto pos = ManagerComponent::getManager<InputManagerComponent>()->getMouseScrollValue();
			if(pos!=0)
			std::cout << pos << std::endl;
		

		if (timing.frame_ == 300)
		{
			animation_name_ = "left_walk";
		}
		updateAnimation(timing);
		updateMesh();
	}

	void SpriteComponent::display(Renderer* renderer, const Timing& dt)
	{
		if (window_to_render_.expired())
		{
			EntitySystem::remove(owner().shared_from_this());
		}
		else
		{
			texture_->bind();
			renderer->setData("u_color", color_);
			renderer->render(vertex_buffer_, index_buffer_, window_to_render_.lock().get(), view_to_render_);
		}
	}
	void SpriteComponent::updateAnimation(const Timing& timing)
	{
		animation_dt_ += timing.dt_.count();
		setRenderRectangle(FloatRect(texture_->getSpriteCoordinate(animation_name_, animation_dt_), texture_->getSpriteSize(animation_name_)));
	}

	void SpriteComponent::updateMesh()
	{
		const PositionComponent* position = owner().getComponent<PositionComponent>();
		
		//Temporary
		auto mapped_vertices = vertex_buffer_.mapVertices(0, 4);
		mapped_vertices[0] = { offset_ + position->getWorldPosition() + points_[0], texture_coords_[0] };
		mapped_vertices[1] = { offset_ + position->getWorldPosition() + points_[1], texture_coords_[1] };
		mapped_vertices[2] = { offset_ + position->getWorldPosition() + points_[2], texture_coords_[2] };
		mapped_vertices[3] = { offset_ + position->getWorldPosition() + points_[3], texture_coords_[3] };

	}
	void SpriteComponent::setRenderRectangle(const FloatRect& render_rect)
	{
		const unsigned int texture_width = texture_->getWidth();
		const unsigned int texture_height = texture_->getHeight();
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
		points_[0].x = -scale_.x * (size_.x / 2.f);
		points_[0].y = -scale_.y * (size_.y / 2.f);
		
		points_[1].x = -scale_.x * (size_.x / 2.f);
		points_[1].y =  scale_.y * (size_.y / 2.f);
		
		glm::vec3 axis(0.0f, 0.0f, 1.0f);
		float angle = glm::radians(rotation_angle_);

		glm::quat rotation = glm::angleAxis(angle, glm::normalize(axis));
		points_[0] = rotation * points_[0];
		points_[1] = rotation * points_[1];

		points_[2] = -points_[0];
		points_[3] = -points_[1];
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

	void SpriteComponent::initTexture(const Description& value, bool interpolate)
	{
		texture_ = ManagerComponent::getManager<TextureManagerComponent>()->getTexture2D(constants::SPRITE_PATH + value.as<std::string>(), interpolate);
	}

	void SpriteComponent::initWindow(const Description& value)
	{

		window_to_render_ = ManagerComponent::getManager<WindowManagerComponent>()->getWindow(value.as<std::string>());
		if(window_to_render_.expired())
		{
			window_to_render_ = ManagerComponent::getManager<WindowManagerComponent>()->getMainWindow();
		}
	}

	void SpriteComponent::initView(const Description& value)
	{
		view_to_render_ = ManagerComponent::getManager<ViewManagerComponent>()->getView(value.as<std::string>());
	
		if(view_to_render_ == nullptr)
		{
			view_to_render_ = ManagerComponent::getManager<ViewManagerComponent>()->getMainView();
		}
	}
	void SpriteComponent::initAngle(const Description& value)
	{
		rotation_angle_ = value.as<float>();
	}
	void SpriteComponent::initAnimation(const Description& value)
	{
		animation_name_ = value.as<std::string>();
	}
}