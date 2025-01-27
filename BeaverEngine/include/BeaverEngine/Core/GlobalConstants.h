#pragma once
#include "BeaverEngine/Core/std.h"
#include "BeaverEngine/Enums/E_RenderDimension.h"
namespace bv
{
	namespace constants
	{
		const std::string DATA_PATH = "./data/";
		const std::string SPRITES_PATH = "./data/sprites/";
		const std::string SHADERS_PATH = "./data/shaders/";
		const std::string SOUNDS_PATH = "./data/sounds/";
		constexpr E_RenderDimension::Type RENDER_MODE = E_RenderDimension::RENDER_2D;
	}
}