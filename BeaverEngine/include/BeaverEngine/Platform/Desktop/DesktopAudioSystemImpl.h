#pragma once
#include "BeaverEngine/System/AudioSystemImpl.h"
#include "BeaverEngine/Platform/Desktop/DesktopSoundBuffer.h">
#include "BeaverEngine/Platform/Desktop/DesktopSoundSource.h">
#include "BeaverEngine/Platform/Desktop/DesktopSoundDevice.h">
#include "BeaverEngine/Platform/Desktop/DesktopMusicPlayer.h">
#include "BeaverEngine/Utils/AudioData.h"

#include "BeaverEngine/Core/Debug.h"
namespace bv
{
	class DesktopAudioSystemImpl
		: public AudioSystemImpl
	{
	public:
		DesktopAudioSystemImpl()
		{
			context_ = DesktopSoundDevice::getInstance();
			sounds_.reserve(SOUND_COUNT);
			for (size_t i = 0; i < SOUND_COUNT; i++)
			{
				available_sounds_indices_[i] = i;
				sounds_.emplace_back().setup();
			}
			music_.reserve(255 - SOUND_COUNT);
		}

		DesktopAudioSystemImpl(const DesktopAudioSystemImpl&) = default;
		DesktopAudioSystemImpl& operator=(const DesktopAudioSystemImpl&) = default;
		DesktopAudioSystemImpl(DesktopAudioSystemImpl&&) = default;
		DesktopAudioSystemImpl& operator=(DesktopAudioSystemImpl&&) = default;

		void iterate(const Timing& dt) override
		{
			for (size_t i = 0; i < std::size(used_sound_indices_); i++)
			{
				if (sounds_[used_sound_indices_[i]].getState() == AudioState::STOPPED)
				{
					sounds_[used_sound_indices_[i]].resetOwner();
					sounds_[used_sound_indices_[i]].stop(nullptr);
					first_available_sound_index_--;
					available_sounds_indices_[first_available_sound_index_] = used_sound_indices_[i];

					used_sound_indices_.erase(used_sound_indices_.begin() + i);
					i--;
				}
			}
			for (auto& music : music_)
			{
				music.update();
			}
		}

		AudioPlayer* playSound(const AudioData& sound, const void* owner) override
		{
			if (first_available_sound_index_ < SOUND_COUNT)
			{
				const size_t sound_index = available_sounds_indices_[first_available_sound_index_];
				AudioPlayer* player_sound = &(sounds_[sound_index]);
				used_sound_indices_.push_back(sound_index);

				sounds_[sound_index].play(buffers_.at(sound.getPath()).first.get(), sound, owner);
				first_available_sound_index_++;
				return player_sound;
			}
			return nullptr;
		}

		AudioPlayer* playMusic(const AudioData& sound, const void* owner) override
		{
			size_t music_index = -1;
			for (size_t i = 0; i < std::size(music_); i++)
			{
				if (music_[i].getState() == AudioState::STOPPED)
				{
					music_index = i;
					break;
				}
			}
			if (music_index == -1)
			{
				music_index = std::size(music_);
				music_.emplace_back();
				music_.back().setup();
			}
			music_[music_index].play(sound, owner);
			return &music_[music_index];
		}


		void registerSound(const AudioData* sound) override
		{
			if (!buffers_.contains(sound->getPath()))
			{
				buffers_.insert({ sound->getPath(), std::pair<std::shared_ptr<DesktopSoundBuffer>, unsigned int>(std::make_shared<DesktopSoundBuffer>(sound->getPath()), 0) });
			}
			buffers_.at(sound->getPath()).second++;
		}

		void soundRemoved(const AudioData* sound) override
		{
			buffers_.at(sound->getPath()).second--;
			if (buffers_.at(sound->getPath()).second == 0)
			{
				buffers_.erase(sound->getPath());
			}

		}

		void clean() override
		{
			sounds_.clear();
			music_.clear();
			buffers_.clear();
		}
	private:

		std::unordered_map<std::string, std::pair<std::shared_ptr<DesktopSoundBuffer>, unsigned int>> buffers_{};
		static constexpr size_t SOUND_COUNT = 245;
		std::vector<DesktopSoundSource> sounds_{};
		std::vector<DesktopMusicPlayer> music_{};
		
		std::array<size_t, SOUND_COUNT> available_sounds_indices_{};
		std::vector<size_t> used_sound_indices_{};
		size_t first_available_sound_index_ = 0;

		std::mutex mutex_{};
		DesktopSoundDevice* context_{};
	};
}