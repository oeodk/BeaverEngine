#pragma once
#include "BeaverEngine/Core/std.h"
#include "BeaverEngine/Core/TypeDef.h"
#include "yaml-cpp/yaml.h"
namespace bv
{
    class Entity;

    struct ComponentDescription
    {
        ComponentDescription(const Description& descr)
        {
            for (auto& param : descr)
            {
                parameters.insert({ param["name"].as<std::string>(), param["value"] });
            }
        }
        std::map<std::string, Description> parameters;
    };

    class  Component
	{
        using CreateFnType = std::function<std::unique_ptr<Component>(Entity&)>;

	protected :
		Entity& parent_;
		bool enabled_{ true };

		static void Register(std::string_view type, const CreateFnType& create_funcion);

		static inline std::unordered_map<std::string, Component::CreateFnType> component_registry_;

        virtual void OnEnabled() {}
        virtual void OnDisabled() {}

	public :
		Component(Entity& parent) : parent_(parent) {}
		virtual ~Component() = default;

        Component(const Component&) = default;
        Component& operator=(const Component&) = default;
        Component(Component&&) = default;
        Component& operator=(Component&&) = default;

        virtual void setup(const ComponentDescription& init_value) = 0;

		// Register the type (std::string T::Type), with the corresponding creation function
		// This let the Create function below be able to create a component with only its name
		template<typename T>
		static void Register()
		{
			Register(T::type_, [](Entity& owner) { return std::make_unique<T>(owner); });
		}

		static std::unique_ptr<Component> Create(std::string_view type, Entity& parent);

        virtual void resolve() {}

		bool enabled() const { return enabled_; }
		Entity& owner() const { return parent_; }

        void SetEnabled(bool enabled)
        {
            if (enabled)
            {
                enable();
            }
            else
            {
                disable();
            }
        }

        void enable()
        {
            if (!enabled_)
            {
                enabled_ = true;
                OnEnabled();
            }
        }
        void disable()
        {
            if (enabled_)
            {
                enabled_ = false;
                OnDisabled();
            }
        }
	};
}

