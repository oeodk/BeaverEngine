#pragma once
#include "BeaverEngine/Utils/AudioManagerImpl.h"
#include "BeaverEngine/Platform/Desktop/DesktopSoundBuffer.h">
#include "BeaverEngine/Platform/Desktop/DesktopSoundSource.h">
#include "BeaverEngine/Platform/Desktop/DesktopSoundDevice.h">
#include "BeaverEngine/Utils/Sound.h"
namespace bv
{
	class DesktopAudioManagerComponent
		: public AudioManagerImpl
	{
	public:
		DesktopAudioManagerComponent()
		{
			context_ = DesktopSoundDevice::getInstance();
			sounds_.reserve(SOUND_COUNT);
			for (size_t i = 0; i < SOUND_COUNT; i++)
			{
				available_sounds_indices_[i] = i;
				sounds_.emplace_back().setup();
			}
		}

		void update(const Timing& dt) override
		{
			for (size_t i = 0; i < first_available_sound_index_; i++)
			{
				if (sounds_[used_sound_indices_[i]].getState() == SoundPlayer::STOPPED)
				{
					sounds_[used_sound_indices_[i]].resetOwner();
					first_available_sound_index_--;
					available_sounds_indices_[first_available_sound_index_] = used_sound_indices_[i];
				}
			}
		}

		SoundPlayer* playSound(const Sound& sound, const void* owner) override
		{
			if (first_available_sound_index_ < SOUND_COUNT)
			{
				const size_t sound_index = available_sounds_indices_[first_available_sound_index_];

				SoundPlayer* player_sound = &(sounds_[sound_index]);
				used_sound_indices_[first_available_sound_index_] = sound_index;
				sounds_[sound_index].play(buffers_.at(sound.getPath()).first.get(), sound, owner);
				first_available_sound_index_++;
				return player_sound;
			}
			return nullptr;
		}

		void registerSound(const Sound* sound) override
		{
			if (!buffers_.contains(sound->getPath()))
			{
				buffers_.insert({ sound->getPath(), std::pair<std::shared_ptr<DesktopSoundBuffer>, unsigned int>(std::make_shared<DesktopSoundBuffer>(sound->getPath()), 0) });
			}
			buffers_.at(sound->getPath()).second++;
		}

		void soundRemoved(const Sound* sound) override
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
			buffers_.clear();
		}
	private:

		std::unordered_map<std::string, std::pair<std::shared_ptr<DesktopSoundBuffer>, unsigned int>> buffers_{};
		static constexpr size_t SOUND_COUNT = 200;
		std::vector<DesktopSoundSource> sounds_{};
		std::array<size_t, SOUND_COUNT> available_sounds_indices_{};
		std::array<size_t, SOUND_COUNT> used_sound_indices_{};
		size_t first_available_sound_index_ = 0;

		std::mutex mutex_{};
		DesktopSoundDevice* context_{};
	};
}