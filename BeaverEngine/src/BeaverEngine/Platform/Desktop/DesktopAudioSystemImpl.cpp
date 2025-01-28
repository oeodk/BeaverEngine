#include "BeaverEngine/Platform/Desktop/DesktopAudioSystemImpl.h"

namespace bv
{
	std::unique_ptr<AudioSystemImpl> AudioSystemImpl::create()
	{
		return std::make_unique<DesktopAudioSystemImpl>();
	}
}