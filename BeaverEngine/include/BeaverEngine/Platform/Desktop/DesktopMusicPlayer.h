#pragma once 
#include "BeaverEngine/Utils/AudioPlayer.h"
#include "BeaverEngine/Core/std.h"
#include <AL/al.h>
#include <sndfile.h>

namespace bv
{
    class AudioData;

	class DesktopMusicPlayer
		: public AudioPlayer
	{
    public:
        DesktopMusicPlayer() = default;
        DesktopMusicPlayer(const DesktopMusicPlayer&) = default;
        DesktopMusicPlayer(DesktopMusicPlayer&&) = default;
        DesktopMusicPlayer& operator=(const DesktopMusicPlayer&) = default;
        DesktopMusicPlayer& operator=(DesktopMusicPlayer&&) = default;

        void setup();
        ~DesktopMusicPlayer();

        void update();

        void play(const AudioData& sound, const void* owner);
        void resume(const void* owner) const override
        {
            if (owner_ == owner)
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
                keep_playing = false;
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
        void loadFile(const std::string& path);
        void startMusic();

        ALuint source_{};
        static const int BUFFER_SAMPLES = 8192;
        static const int NUM_BUFFERS = 4;
        ALuint buffers_[NUM_BUFFERS] = {};
        SNDFILE* sndFile_{};
        SF_INFO sfinfo_{};
        short* membuf_{};
        ALenum format_{};

        bool keep_playing = false;

        inline static const std::map<ALint, AudioState::AudioState> AL_STATE_TO_STATE_ = { {AL_PLAYING, AudioState::PLAYING}, {AL_PAUSED, AudioState::PAUSED}, {AL_STOPPED, AudioState::STOPPED} };

	};
}