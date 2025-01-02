#pragma once
#include "BeaverEngine/std.h"
#include "BeaverEngine/System/System.h"
#include "BeaverEngine/Window.h"

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
		float fps_ = 30;

		std::unique_ptr<Window> window_;

	public :
		Game(const WindowProperties& props = WindowProperties());
		Game(const Game&) = default;
		Game(Game&&) = default;
		Game& operator=(const Game&) = default;
		Game& operator=(Game&&) = default;

		static bool& closeApplication()
		{
			static bool close{};
			return close;
		}
		static void close()
		{
			closeApplication() = true;
		}

		virtual ~Game() = default;

		virtual void setupSystem() = 0;
		virtual void registerComponent() const = 0;

		void setInitFunction(InitFunction init_fuction) { init_function_ = init_fuction; }

		bool run();
	};
}

