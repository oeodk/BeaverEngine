#include "BeaverEngine/Utils/Sound.h"
#include "BeaverEngine/Component/AudioManagerComponent.h"

namespace bv
{
	void Sound::setup(std::string_view path)
	{
		path_ = std::string(path);
		ManagerComponent::getManager<AudioManagerComponent>()->registerSound(this);
	}

	Sound::~Sound()
	{
		if(auto* AudioManager = ManagerComponent::getManager<AudioManagerComponent>())
		{
			AudioManager->soundRemoved(this);
		}
	}
}

