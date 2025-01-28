#pragma once
#include <AL/alc.h>
namespace bv
{
	class DesktopSoundDevice
	{
	public:
		static DesktopSoundDevice* getInstance()
		{
			static DesktopSoundDevice device;
			return &device;
		}

		DesktopSoundDevice();
		void destroy();
		~DesktopSoundDevice() = default;
	private:
		
		ALCdevice* ALCDevice_;
		ALCcontext* ALCContext_;
	};
}

