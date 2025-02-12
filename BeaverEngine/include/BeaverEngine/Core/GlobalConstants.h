#pragma once
#include "BeaverEngine/Core/std.h"
#include "BeaverEngine/Enums/RenderDimension.h"
namespace bv
{
	namespace constants
	{
		const std::string DATA_PATH = "./data/";
		const std::string SPRITES_PATH = "./data/sprites/";
		const std::string SHADERS_PATH = "./data/shaders/";
		const std::string SOUNDS_PATH = "./data/sounds/";
		const std::string MUSICS_PATH = "./data/musics/";
		const std::string LEVELS_PATH = "./data/levels/";
		const std::string TEMPLATE_PATH = "./data/templates/";
		constexpr RenderDimension::Type RENDER_MODE = RenderDimension::RENDER_2D;
	}
}