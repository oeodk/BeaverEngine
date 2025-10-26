#pragma once
#include "BeaverEngine/Core/std.h"
#include "BeaverEngine/Core/TypeDef.h"
#include "BeaverEngine/Component/Component.h"
#include "yaml-cpp/yaml.h"

namespace bv
{
    class Entity : public std::enable_shared_from_this<Entity>
    {
    public :
        ~Entity() { components_.clear(); children_by_name_.clear(); }
        static EntityRef create() { return std::make_shared<Entity>(); }
        void addChild(std::string_view name, const EntityRef& child);
        void removeChild(EntityRef child);
        void removeChild(Entity& child) { removeChild(child.shared_from_this()); }

        void removeChildren();
        
        void resolve()
        {
            for (auto& component : components_)
            {
                component.second->resolve();
            }
        }

        Component* addComponent(std::string_view type, const ComponentDescription& init_value);
        template<typename T>
        T* addComponent(const typename T::init_values& initial_value)
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
            auto component = components_.find(type);
            if (component == components_.end())
            {
                return nullptr;
            }
            return component->second.get();
        }

        bool active() const { return active_; }

        void setActive(bool active)
        { 
            active_ = active; 
            for (auto& child : childrens_)
            {
                child->setActive(active);
            }
        }

        EntityRef getChild(std::string_view name) const
        {
            if (children_by_name_.size() == 0)
            {
                return nullptr;
            }
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

        const std::string& getTag() const { return tag_; }
        void setTag(const std::string& tag) { tag_ = tag; }

    private:
        bool active_{ true };
        std::weak_ptr<Entity> parent_;
        std::vector<EntityRef> childrens_;
        std::map<std::string, std::weak_ptr<Entity>> children_by_name_;

        std::string tag_{};

        struct StringHash {
            using is_transparent = void; // enables heterogeneous lookup
            std::size_t operator()(std::string_view key) const noexcept {
                return std::hash<std::string_view>{}(key);
            }
        };

        struct StringEqual {
            using is_transparent = void; // enables heterogeneous lookup
            bool operator()(std::string_view lhs, std::string_view rhs) const noexcept {
                return lhs == rhs;
            }
        };

        std::unordered_map<std::string, std::unique_ptr<Component>, StringHash, StringEqual> components_;

    };
}

