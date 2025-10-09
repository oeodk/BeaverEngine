#pragma once
#include "BeaverEngine/Component/LogicComponent.h"

namespace bv
{
	class MousePointerComponent
		: public LogicComponent
	{
	public:
		static constexpr auto type_ = "MousePointer";

		MousePointerComponent(Entity& owner) : Component(owner) {}
		~MousePointerComponent(){}
		MousePointerComponent(const MousePointerComponent&) = default;
		MousePointerComponent& operator=(const MousePointerComponent&) = default;
		MousePointerComponent(MousePointerComponent&&) = default;
		MousePointerComponent& operator=(MousePointerComponent&&) = default;

		void setup(const ComponentDescription& init_value) override;
		void resolve() override;

		

		void updateLogic(const Timing& timing) override;

	};

}

