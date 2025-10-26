#pragma once
#include "BeaverEngine/Component/LevelComponent.h"
#include <nlohmann/json.hpp>

namespace bv
{
	class LdtkLevelComponent
		: public LevelComponent
	{
	public :
		static constexpr auto type_ = "LdtkLevel";

		LdtkLevelComponent(Entity& owner) : LevelComponent(owner), Component(owner) {}
		LdtkLevelComponent(const LdtkLevelComponent&) = default;
		LdtkLevelComponent& operator=(const LdtkLevelComponent&) = default;
		LdtkLevelComponent(LdtkLevelComponent&&) = default;
		LdtkLevelComponent& operator=(LdtkLevelComponent&&) = default;

		void onCollision(const ColliderComponent& other) override;

		void setup(const ComponentDescription& init_value) override;
		void resolve() override;

	protected:
		void createLayer(bv::Description& layer_descr, const std::string& folder_path, const std::string& layer_name, const bv::ComponentDescription& init_value, const std::string& layer_image);
		nlohmann::json level_descr_;
	};
}
