#pragma once
#include "BeaverEngine/Utils/AudioPlayer.h"
#include "BeaverEngine/Core/std.h"
#include <AL/al.h>
namespace bv
{
    class AudioData;
    class DesktopSoundBuffer;

    class DesktopSoundSource
        : public AudioPlayer
    {
    public:
        DesktopSoundSource() = default;
        void setup();
        ~DesktopSoundSource();

        DesktopSoundSource(const DesktopSoundSource&) = default;
        DesktopSoundSource(DesktopSoundSource&&) = default;
        DesktopSoundSource& operator=(const DesktopSoundSource&) = default;
        DesktopSoundSource& operator=(DesktopSoundSource&&) = default;

        void play(const DesktopSoundBuffer* buffer, const AudioData& sound, const void* owner);
        void resume(const void* owner) const override 
        {
            if(owner_ == owner) 
                alSourcePlay(source_); 
        }
        void pause(const void* owner) const override 
        {
            if (owner_ == owner)
                alSourcePause(source_); 
        }
        
        void stop(const void* owner) override 
        {
            if (owner_ == owner)
            {
                alSourceStop(source_);
                owner_ = nullptr;
            }
        }
        AudioState::AudioState getState() const override
        {
            ALint state;
            alGetSourcei(source_, AL_SOURCE_STATE, &state);
            return AL_STATE_TO_STATE_.at(state);
        }
        void resetOwner() { owner_ = nullptr; }
    private:
        ALuint source_{};
		ALuint buffer_ = 0;

        inline static const std::map<ALint, AudioState::AudioState> AL_STATE_TO_STATE_ = { {AL_PLAYING, AudioState::PLAYING}, {AL_PAUSED, AudioState::PAUSED}, {AL_STOPPED, AudioState::STOPPED} };
    };
}

