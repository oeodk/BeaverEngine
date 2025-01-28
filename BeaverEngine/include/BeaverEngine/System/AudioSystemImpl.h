#pragma once
#include "BeaverEngine/Core/std.h"
#include "BeaverEngine/Utils/Timing.h"
#include "BeaverEngine/Utils/Sound.h"
#include "BeaverEngine/Utils/SoundPLayer.h"
namespace bv
{
	class AudioSystemImpl
	{
	public:
        AudioSystemImpl() = default;
        AudioSystemImpl(const AudioSystemImpl&) = default;
        AudioSystemImpl(AudioSystemImpl&&) = default;
        AudioSystemImpl& operator=(const AudioSystemImpl&) = default;
        AudioSystemImpl& operator=(AudioSystemImpl&&) = default;

        virtual ~AudioSystemImpl() = default;

        virtual void iterate(const Timing& dt) = 0;

        // owner : object owning the sound
        virtual SoundPlayer* playSound(const Sound& sound, const void* owner) = 0;
        virtual void registerSound(const Sound* sound) = 0;
        virtual void soundRemoved(const Sound* sound) = 0;
        virtual void clean() = 0;

        static std::unique_ptr<AudioSystemImpl> create();
	};
}