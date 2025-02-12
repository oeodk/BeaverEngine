#pragma once
#include "BeaverEngine/System/System.h"
#include "BeaverEngine/Utils/View2D.h"

namespace bv
{
	class ViewSystem
		: public System
	{
	public:
        ViewSystem() = default;
        ViewSystem(const ViewSystem&) = default;
        ViewSystem(ViewSystem&&) = default;
        ViewSystem& operator=(const ViewSystem&) = default;
        ViewSystem& operator=(ViewSystem&&) = default;

        ~ViewSystem() = default;

        static ViewSystem& getInstance();

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
                main_view_id_ = view_name;
            }
            return views_.at(std::string(view_name)).get();
        }

        void iterate(const Timing& dt) override {}

        View2D* getMainView() const { return main_view_; }
        const std::string& getMainViewId() const { return main_view_id_; }

    private:
        std::unordered_map<std::string, std::unique_ptr<View2D>> views_;
        View2D* main_view_ = nullptr;
        std::string main_view_id_;
    };
}

