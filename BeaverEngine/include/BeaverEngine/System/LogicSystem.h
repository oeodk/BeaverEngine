#pragma once
#include "BeaverEngine/System/System.h"
#include "BeaverEngine/Component/LogicComponent.h"

namespace bv
{
    class LogicSystem :
        public System
    {
        public:
            LogicSystem() = default;
            LogicSystem(const LogicSystem&) = default;
            LogicSystem(LogicSystem&&) = default;
            LogicSystem& operator=(const LogicSystem&) = default;
            LogicSystem& operator=(LogicSystem&&) = default;

            ~LogicSystem() = default;

            void iterate(const Timing& dt) override;

            void registerComponent(LogicComponent* logic_component, bool parallelize = false);
            void removeComponent(LogicComponent* logic_component, bool parallelize = false);

            static LogicSystem& getInstance();
        private:
            std::unordered_set<LogicComponent*> logic_components_;
            std::unordered_set<LogicComponent*> parallelized_logic_components_;
        
    };
}

