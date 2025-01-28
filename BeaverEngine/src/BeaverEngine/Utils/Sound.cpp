#include "BeaverEngine/Utils/Sound.h"
#include "BeaverEngine/System/AudioSystem.h"

namespace bv
{
	void Sound::setup(std::string_view path)
	{
		path_ = std::string(path);
		AudioSystem::getInstance().registerSound(this);
	}

	Sound::~Sound()
	{
		AudioSystem::getInstance().soundRemoved(this);
	}
}

