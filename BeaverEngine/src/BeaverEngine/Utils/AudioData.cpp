#include "BeaverEngine/Utils/AudioData.h"
#include "BeaverEngine/System/AudioSystem.h"

namespace bv
{
	void AudioData::setup(std::string_view path)
	{
		path_ = std::string(path);
		AudioSystem::getInstance().registerSound(this);
	}

	AudioData::~AudioData()
	{
		AudioSystem::getInstance().soundRemoved(this);
	}
}

