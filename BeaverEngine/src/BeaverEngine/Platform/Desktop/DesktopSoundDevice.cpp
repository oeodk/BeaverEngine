#include "BeaverEngine/Platform/Desktop/DesktopSoundDevice.h"
#include "BeaverEngine/Component/AudioManagerComponent.h"
#include <AL/al.h>
namespace bv
{
	DesktopSoundDevice::DesktopSoundDevice()
	{
		ALCDevice_ = alcOpenDevice(nullptr); // nullptr = get default device
		if (!ALCDevice_)
			throw("failed to get sound device");

		ALCContext_ = alcCreateContext(ALCDevice_, nullptr);  // create context
		if (!ALCContext_)
			throw("Failed to set sound context");

		if (!alcMakeContextCurrent(ALCContext_))   // make context current
			throw("failed to make context current");

		const ALCchar* name = nullptr;
		if (alcIsExtensionPresent(ALCDevice_, "ALC_ENUMERATE_ALL_EXT"))
			name = alcGetString(ALCDevice_, ALC_ALL_DEVICES_SPECIFIER);
		if (!name || alcGetError(ALCDevice_) != AL_NO_ERROR)
			name = alcGetString(ALCDevice_, ALC_DEVICE_SPECIFIER);
	}

	DesktopSoundDevice::~DesktopSoundDevice()
	{
		ManagerComponent::getManager<AudioManagerComponent>()->clean();

		if (!alcMakeContextCurrent(nullptr))
			throw("failed to set context to nullptr");

		alcDestroyContext(ALCContext_);

		if (!alcCloseDevice(ALCDevice_))
			throw("failed to close sound device");
	}
}