#pragma once
#include "BeaverEngine/Core/std.h"
#include "BeaverEngine/Core/TypeDef.h"
#include "BeaverEngine/Component/Component.h"
#include "yaml-cpp/yaml.h"

namespace bv
{
    class Entity : public std::enable_shared_from_this<Entity>
    {
    private:
        bool active_{true};
        std::weak_ptr<Entity> parent_;
        std::vector<EntityRef> childrens_;
        std::map<std::string, std::weak_ptr<Entity>> children_by_name_;
        std::unordered_map<std::string, std::unique_ptr<Component>> components_;
    public :
        ~Entity() {}
        static EntityRef create() { return std::make_shared<Entity>(); }
        void addChild(std::string_view name, const EntityRef& child);
        void removeChild(EntityRef child);
        void removeChild(Entity& child) { removeChild(child.shared_from_this()); }

        Component* addComponent(std::string_view type, const ComponentDescription& init_value);
        template<typename T>
        typename T* addComponent(const typename T::init_values& initial_value)
        {
            auto result = components_.emplace(T::type, std::make_unique<T>(*this));
            auto iterator = result.first;
            T* comp = dynamic_cast<T*>(iterator->second.get());
            comp->setup(initial_value);
        }

        template<typename T>
        T* getComponent() const
        {
            return dynamic_cast<T*>(getComponent(T::type_));
        }

        Component* getComponent(std::string_view type) const
        {
            auto component = components_.find((std::string)type);
            if (component == components_.end())
            {
                return nullptr;
            }
            return component->second.get();
        }

        bool active() const { return active_; }

        void setActive(bool active) { active_ = active; }

        EntityRef getChild(std::string_view name) const
        {
            auto child = children_by_name_.find((std::string)name);
            if (child == children_by_name_.end())
            {
                return nullptr;
            }
            return child->second.lock();
        }

        EntityRef getParent() const { return parent_.lock(); }

        void clear();
        
        std::span<EntityRef> getChildren() { return childrens_; }

    };
}

