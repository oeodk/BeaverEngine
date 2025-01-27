#pragma once
#include "BeaverEngine/Core/std.h"
#include "BeaverEngine/Utils/Timing.h"
#include "BeaverEngine/Utils/Sound.h"
#include "BeaverEngine/Utils/SoundPLayer.h"
namespace bv
{
	class AudioManagerImpl
	{
	public:
        AudioManagerImpl() = default;
        AudioManagerImpl(const AudioManagerImpl&) = default;
        AudioManagerImpl(AudioManagerImpl&&) = default;
        AudioManagerImpl& operator=(const AudioManagerImpl&) = default;
        AudioManagerImpl& operator=(AudioManagerImpl&&) = default;

        virtual ~AudioManagerImpl() = default;

        virtual void update(const Timing& dt) = 0;

        // owner : object owning the sound
        virtual SoundPlayer* playSound(const Sound& sound, const void* owner) = 0;
        virtual void registerSound(const Sound* sound) = 0;
        virtual void soundRemoved(const Sound* sound) = 0;
        virtual void clean() = 0;

        static std::unique_ptr<AudioManagerImpl> create();
	};
}