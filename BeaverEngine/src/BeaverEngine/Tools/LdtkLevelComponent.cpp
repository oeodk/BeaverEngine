#include "BeaverEngine/Tools/LdtkLevelComponent.h"
#include "BeaverEngine/Core/Scene.h"
#include "BeaverEngine/Core/std.h"
#include "BeaverEngine/Core/GlobalConstants.h"
#include "BeaverEngine/Core/Entity.h"
#include "BeaverEngine/Component/PositionComponent.h"
#include "BeaverEngine/Component/LayerComponent.h"

#include "BeaverEngine/System/WindowSystem.h"
#include "BeaverEngine/System/ViewSystem.h"

#include "BeaverEngine/Component/ColliderComponent.h"
#include "BeaverEngine/Component/IntGridColliderComponent.h"
#include "BeaverEngine/Component/PositionComponent.h"
#include "yaml-cpp/yaml.h"

namespace bv
{
	void LdtkLevelComponent::onCollision(const ColliderComponent& other)
	{
		auto& collider_tile_map = owner().getComponent<IntGridColliderComponent>()->getColliderTileMap();
		auto other_position = other.owner().getComponent<PositionComponent>();
		other_position->move(collider_tile_map.at(&other));
		collider_tile_map.erase(&other);
	}

	void LdtkLevelComponent::setup(const ComponentDescription& init_value)
	{
		std::string folder_path = init_value.parameters.at("folderPath").as<std::string>();
		std::fstream level_file(constants::LEVELS_PATH + folder_path + "/data.json");
		level_file >> level_descr_;
		level_file.close();

		Description layer_descr = Descr::load(constants::TEMPLATE_PATH + "LevelLayer.yaml");

		if (init_value.parameters.at("customLayerHeight").as<bool>())
		{
			for (auto& layer : level_descr_["layers"])
			{
				std::string layer_image = layer.get<std::string>();
				std::string layer_name = layer_image.substr(0, layer_image.length() - 4);

				createLayer(layer_descr, folder_path, layer_name, init_value, layer_image);
			}
		}
		else
		{
			createLayer(layer_descr, folder_path, "_composite", init_value, "_composite.png");
		}

		level_width_ = level_descr_["width"].get<unsigned int>();
		level_height_ = level_descr_["height"].get<unsigned int>();

		for (auto& [entityName, entityList] : level_descr_["entities"].items())
		{
			for (const auto& entity : entityList)
			{
				Description entity_descr = Descr::load(constants::TEMPLATE_PATH + entityName + ".yaml");

				entity_descr[0]["description"]["components"]["Position"]["x"] = entity["x"].dump();
				entity_descr[0]["description"]["components"]["Position"]["y"] = entity["y"].dump();

				// Extract custom fields
				if (entity.contains("customFields") && entity["customFields"].is_object())
				{
					for (auto& [fieldName, fieldValue] : entity["customFields"].items())
					{
						size_t pos = fieldName.find_first_of('_');
						std::string component_name = fieldName.substr(0,pos);
						std::string field_name = fieldName.substr(pos + 1);
						if (fieldValue.is_array())
						{
							size_t i = 0;
							for (const auto& val : fieldValue)
							{
								entity_descr[0]["description"]["components"][component_name][field_name][i] = val.dump();
								i++;
							}
						}
						else
						{
							entity_descr[0]["description"]["components"][component_name][field_name] = fieldValue.dump();
						}
					}
				}
				Scene::CreateChild(entity_descr[0], owner().shared_from_this());
			}
		}
	}

	void LdtkLevelComponent::createLayer(bv::Description& layer_descr, const std::string& folder_path, const std::string& layer_name, const bv::ComponentDescription& init_value, const std::string& layer_image)
	{
		layer_descr[0]["name"] = folder_path + layer_name;
		layer_descr[0]["description"]["components"]["Position"]["x"] = 0;
		layer_descr[0]["description"]["components"]["Position"]["y"] = 0;
		layer_descr[0]["description"]["components"]["Position"]["z"] = init_value.parameters.at(layer_name).as<float>();

		if (init_value.parameters.contains("window"))
		{
			layer_descr[0]["description"]["components"]["Layer"]["window"] = init_value.parameters.at("window").as<std::string>();
		}
		else
		{
			layer_descr[0]["description"]["components"]["Layer"]["window"] = WindowSystem::getInstance().getMainWindowId();
		}
		if (init_value.parameters.contains("view"))
		{
			layer_descr[0]["description"]["components"]["Layer"]["view"] = init_value.parameters.at("view").as<std::string>();
		}
		else
		{
			layer_descr[0]["description"]["components"]["Layer"]["view"] = ViewSystem::getInstance().getMainViewId();
		}
		layer_descr[0]["description"]["components"]["Layer"]["texture"] = constants::LEVELS_PATH + folder_path + "/" + layer_image;

		layer_descr[0]["description"]["components"]["Sprite"]["layer"] = layer_descr[0]["name"];
		
		Scene::CreateChild(layer_descr[0], owner().shared_from_this());

	}

	void LdtkLevelComponent::resolve()
	{
		auto position = owner().getComponent<PositionComponent>()->getWorldPosition();
		for (auto& child : owner().getChildren())
		{
			if (child->getComponent<LayerComponent>())
			{
				child->getComponent<PositionComponent>()->setRelativePosition(0, 0);
			}
			else
			{
				const auto& child_pos = child->getComponent<PositionComponent>()->getWorldPosition();
				child->getComponent<PositionComponent>()->setRelativePosition(-static_cast<float>(level_width_) / 2.f + child_pos.x, static_cast<float>(level_height_) / 2.f - child_pos.y);

			}
		}
	}
}