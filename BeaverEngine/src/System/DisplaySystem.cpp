#include "BeaverEngine/System/DisplaySystem.h"
#include "BeaverEngine/Core/Renderer.h"
#include "BeaverEngine/Core/Entity.h"
#include "BeaverEngine/Core/GlobalConstants.h"
namespace bv
{
	DisplaySystem& DisplaySystem::getInstance()
	{
		static DisplaySystem display_system;
		return display_system;
	}

	DisplaySystem::DisplaySystem()
		: renderer_(Renderer::getInstance())
	{
	}

	void DisplaySystem::setup()
	{
		renderer_->setup();
	}

	void DisplaySystem::iterate(const Timing& dt)
	{
		// Maybe sort based on transparency
		for (auto& display_component : display_components_)
		{
			if (display_component->enabled() && display_component->owner().active())
			{
				switch (constants::RENDER_MODE)
				{
				case E_RenderDimension::RENDER_2D:
					renderer_->begin2DRender(display_component->getWindow());
					break;
				case E_RenderDimension::RENDER_3D:
					renderer_->begin3DRender(display_component->getWindow());
					break;
				}
		
				display_component->display(renderer_, dt);
		
				switch (constants::RENDER_MODE)
				{
				case E_RenderDimension::RENDER_2D:
					renderer_->end2DRender();
					break;
				case E_RenderDimension::RENDER_3D:
					renderer_->end3DRender();
					break;
				}
			}
		}
	}

	void DisplaySystem::registerComponent(DisplayComponent* display_component)
	{
		display_components_.insert(display_component);
	}
	void DisplaySystem::removeComponent(DisplayComponent* display_component)
	{
		display_components_.erase(display_component);
	}
}