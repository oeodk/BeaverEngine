#include "BeaverEngine/Scene.h"
#include "BeaverEngine/Entity.h"

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
                children.insert({ child["name"].as<std::string>(), child["description"] });
            }
            for (auto& component : descr["components"])
            {
                children.insert({ component["name"].as<std::string>(), component["param"] });
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

    EntityRef CreateChildInternal(const EntityDescription& descr, const EntityRef& parent, std::string_view name)
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
        return new_entity;
    }

    EntityRef CreateChild(const Description& descr, const EntityRef& parent)
    {
        return CreateChildInternal(descr["description"], parent, descr["name"].as<std::string>());
        
    }

    EntityRef Scene::addChildToRoot(const Description& descr)
    {
        return CreateChild(descr, rootEntity());
    }

    void load(void(*init_function)())
    {
        init_function();
    }

    void clear()
    {
        rootEntity()->clear();
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
}