#pragma once
#include "BeaverEngine/Component/ManagerComponent.h"
#include "BeaverEngine/Utils/View2D.h"

namespace bv
{
	class ViewManagerComponent
		: public ManagerComponent
	{
	public:
		static constexpr auto type_ = "ViewManager";

		ViewManagerComponent(Entity& owner) : ManagerComponent(owner) {}
        void setup(const ComponentDescription& init_value) override
        {
            for (const auto& view_parameter : init_value.parameters)
            {
                std::string view_name;
                std::stringstream ss(view_parameter.first);
                std::getline(ss, view_name, '.');
                View2D* view = getView(view_name);

                if (view == nullptr)
                {
                    view = addView(view_name);
                }
                std::string parameter_name;
                std::getline(ss, parameter_name);

                view->setupParameter(parameter_name, view_parameter.second);
            }
        }

        View2D* getView(std::string_view view_name) const
        {
            auto view = views_.find((std::string)view_name);
            if (view == views_.end())
            {
                return nullptr;
            }
            return view->second.get();
        }

        View2D* addView(std::string_view view_name)
        {
            views_.insert({ std::string(view_name), std::make_unique<View2D>()});
            if (main_view_ == nullptr)
            {
                main_view_ = views_.at(std::string(view_name)).get();
            }
            return views_.at(std::string(view_name)).get();
        }

        void update(const Timing& dt) override {}

        View2D* getMainView() const { return main_view_; }

    private:
        std::unordered_map<std::string, std::unique_ptr<View2D>> views_;
        View2D* main_view_ = nullptr;;
    };
}

