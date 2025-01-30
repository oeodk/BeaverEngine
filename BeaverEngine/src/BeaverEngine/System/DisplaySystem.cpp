#include "BeaverEngine/System/DisplaySystem.h"
#include "BeaverEngine/Component/DisplayComponent.h"
#include "BeaverEngine/Component/PositionComponent.h"

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
		std::vector<DisplayComponent*> sorted_display_component(display_components_.begin(), display_components_.end());
		std::sort(sorted_display_component.begin(), sorted_display_component.end(),
			[](DisplayComponent* a, DisplayComponent* b) {
				return a->owner().getComponent<PositionComponent>()->getWorldPosition()[2] <
					b->owner().getComponent<PositionComponent>()->getWorldPosition()[2];
			});
		// Maybe sort based on transparency
		for (auto& display_component : sorted_display_component)
		{
			if (display_component->enabled() && display_component->owner().active())
			{
				// Maybe move the render mode to the display component
				switch (constants::RENDER_MODE)
				{
				case RenderDimension::RENDER_2D:
					renderer_->begin2DRender(display_component->getWindow());
					break;
				case RenderDimension::RENDER_3D:
					renderer_->begin3DRender(display_component->getWindow());
					break;
				}
		
				display_component->display(renderer_, dt);
		
				switch (constants::RENDER_MODE)
				{
				case RenderDimension::RENDER_2D:
					renderer_->end2DRender();
					break;
				case RenderDimension::RENDER_3D:
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