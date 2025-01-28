#include "BeaverEngine/System/AudioSystem.h"

namespace bv
{
	AudioSystem& AudioSystem::getInstance()
	{
		static AudioSystem audio_system;
		return audio_system;
	}
}
