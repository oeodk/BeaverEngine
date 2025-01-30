#pragma once
#include "BeaverEngine/Component/Component.h"
#include "BeaverEngine/Enums/RenderDimension.h"
#include "BeaverEngine/Utils/Timing.h"

namespace bv
{
	class Renderer;
	class Window;

	class DisplayComponent
		: virtual public Component
	{
	public:
		
		DisplayComponent();
		~DisplayComponent();
		virtual void display(Renderer* renderer, const Timing& dt) = 0;


		RenderDimension::Type dimension_;

		Window* getWindow()const { return window_to_render_.lock().get(); }
	protected:
		std::weak_ptr<Window> window_to_render_;
	};
}

