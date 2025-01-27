#include "BeaverEngine/Platform/Desktop/DesktopAudioManagerImpl.h"

namespace bv
{
	std::unique_ptr<AudioManagerImpl> AudioManagerImpl::create()
	{
		return std::make_unique<DesktopAudioManagerComponent>();
	}
}