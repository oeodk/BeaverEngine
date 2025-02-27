#pragma once
#include "BeaverEngine/Core/std.h"
#include <glm/glm.hpp>
namespace bv
{
	class AudioData
	{
	public:
		AudioData() = default;
		void setup(std::string_view path);

		AudioData(const AudioData&) = default;
		AudioData(AudioData&&) = default;
		AudioData& operator=(const AudioData&) = default;
		AudioData& operator=(AudioData&&) = default;

		~AudioData();

		float getPitch() const { return pitch_; }
		float getGain() const { return gain_; }
		const glm::vec3& getPosition() const { return position_; }
		const glm::vec3& getVelocity() const { return velocity_; }
		bool isLooping() const { return loop_sound_; }

		void setPitch(float pitch) { pitch_ = pitch; }
		void setGain(float gain) { gain_ = gain; }
		void setPosition(const glm::vec3& position) { position_ = position; }
		void setVelocity(const glm::vec3& velocity) { velocity_ = velocity; }
		void setLoop(bool loop) { loop_sound_ = loop; }

		const std::string& getPath() const { return path_; }

	private:
		float pitch_ = 1.f;
		float gain_ = 1.f;
		glm::vec3 position_{};
		glm::vec3 velocity_{};
		bool loop_sound_ = false;

		std::string path_{};
	};
}
