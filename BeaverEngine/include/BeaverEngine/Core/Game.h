#pragma once
#include "BeaverEngine/BeaverEngine.h"
#include "BeaverEngine/Functions/components.h"

namespace bv
{
	class Game
	{
	protected :
		typedef void(*InitFunction)(void);

		std::vector<System*> systems_;

		template<typename T>
		void addSystem()
		{
			systems_.emplace_back(&T::getInstance());
		}
		InitFunction init_function_;
		float fps_ = 60;

		static bool& closeApplication()
		{
			static bool close{};
			return close;
		}
	public :
		Game() = default;
		Game(const Game&) = default;
		Game(Game&&) = default;
		Game& operator=(const Game&) = default;
		Game& operator=(Game&&) = default;


		static void close()
		{
			closeApplication() = true;
		}

		virtual ~Game() = default;

		virtual void setupSystem() = 0;
		virtual void registerComponent() const = 0;

		void setInitFunction(InitFunction init_fuction) { init_function_ = init_fuction; }

#ifndef SHIPPING
		virtual void defineDebugDataPath()
		{
			std::filesystem::path new_path = "../../";
			std::filesystem::current_path(new_path);
		}
#endif // !SHIPPING


		bool run();
	};
}

