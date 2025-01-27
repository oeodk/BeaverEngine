#pragma once
#include "BeaverEngine/Utils/SoundPlayer.h"
#include "BeaverEngine/Core/std.h"
#include <AL/al.h>
namespace bv
{
    class Sound;
    class DesktopSoundBuffer;

    class DesktopSoundSource
        : public SoundPlayer
    {
    public:
        DesktopSoundSource() = default;
        void setup();
        ~DesktopSoundSource();
        void play(const DesktopSoundBuffer* buffer, const Sound& sound, const void* owner);
        void resume(const void* owner) const  override 
        {
            if(owner_ == owner) 
                alSourcePlay(source_); 
        }
        void pause(const void* owner) const  override 
        {
            if (owner_ == owner)
                alSourcePause(source_); 
        }
        SoundState getState() const override
        {
            ALint state;
            alGetSourcei(source_, AL_SOURCE_STATE, &state);
            return AL_STATE_TO_STATE_.at(state);
        }
        void resetOwner() { owner_ = nullptr; }
    private:
        ALuint source_{};
		ALuint buffer_ = 0;

        inline static const std::map<ALint, SoundState> AL_STATE_TO_STATE_ = { {AL_PLAYING, PLAYING}, {AL_PAUSED, PAUSED}, {AL_STOPPED, STOPPED} };
    };
}

