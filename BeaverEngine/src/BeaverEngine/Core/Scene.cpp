#include "BeaverEngine/Core/Scene.h"
#include "BeaverEngine/Core/Entity.h"

namespace bv
{
    class ComponentDescription;
}
namespace bv::Scene
{

    struct EntityDescription
    {
        EntityDescription(const Description& descr)
        {
            for (auto& child : descr["children"])
            {
                children.insert({ child["name"].as<std::string>(), child });
            }
            for (auto& component : descr["components"])
            {
                components.insert({ component.first.as<std::string>(), component.second});
            }
        }
        std::map<std::string, ComponentDescription> components;
        std::map<std::string, Description> children;
    };

    EntityRef rootEntity()
    {
        static auto root = Entity::create();
        return root;
    }

    std::map<std::string, std::vector<Entity*>>& tagMap()
    {
        static std::map<std::string, std::vector<Entity*>> tag_map;
        return tag_map;
    }

    EntityRef CreateChildInternal(const EntityDescription& descr, const EntityRef& parent, std::string_view name, const std::string& tag)
    {
        EntityRef new_entity = Entity::create();
        parent->addChild(name, new_entity);
        for (const auto& component : descr.components)
        {
            new_entity->addComponent(component.first, component.second);
        }

        for (const auto& child : descr.children)
        {
            Scene::CreateChild(child.second, new_entity);
        }
        if(tag != "")
        {
            tagMap()[tag].push_back(new_entity.get());
            new_entity->setTag(tag);
        }
        new_entity->resolve();

        return new_entity;
    }

    EntityRef CreateChild(const Description& descr, const EntityRef& parent)
    {
        std::string tag = "";
        if (descr["tag"])
        {
            tag = descr["tag"].as<std::string>();
        }
        return CreateChildInternal(descr["description"], parent, descr["name"].as<std::string>(), tag);
    }

    EntityRef Scene::addChildToRoot(const Description& descr)
    {
        return CreateChild(descr, rootEntity());
    }

    void load(const Description& descr)
    {
        for (const auto& entity_descr : descr)
        {
            Scene::addChildToRoot(entity_descr);
        }
    }

    void clear()
    {
        rootEntity()->clear();
        tagMap().clear();
    }

    EntityRef FindObjectRecursive(const EntityRef& parent, std::string_view name)
    {
        auto found = parent->getChild(name);
        if (found != nullptr)
        {
            return found;
        }
        for (const auto& c : parent->getChildren())
        {
            found = FindObjectRecursive(c, name);
            if (found)
            {
                return found;
            }
        }
        return {};
    }

    EntityRef findObject(std::string_view name)
    {
        return FindObjectRecursive(rootEntity(), name);
    }

    void removeEntityTag(const EntityRef& entity, const std::string& tag)
    {
        auto& tag_map = tagMap();
        auto& tag_vector = tag_map.at(tag);
        tag_vector.erase(
            std::remove(tag_vector.begin(), tag_vector.end(), entity.get()),
            tag_vector.end()
        );
        if (tag_vector.size() == 0)
        {
            tag_map.erase(tag);
        }
    }
    Entity* getEntityWithTag(std::string_view tag)
    {
        std::string str_tag(tag);
        if (tagMap().contains(str_tag))
        {
            return tagMap().at(str_tag)[0];
        }
        return nullptr;
    }
    std::vector<Entity*>* getEntitiesWithTag(std::string_view tag)
    {
        std::string str_tag(tag);
        if (tagMap().contains(str_tag))
        {
            return &tagMap().at(str_tag);
        }
        return nullptr;
    }
}