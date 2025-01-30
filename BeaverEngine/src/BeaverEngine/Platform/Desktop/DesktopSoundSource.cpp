#include "BeaverEngine/Platform/Desktop/DesktopSoundSource.h"
#include "BeaverEngine/Platform/Desktop/DesktopSoundBuffer.h"
#include "BeaverEngine/Utils/AudioData.h"
namespace bv
{
	void DesktopSoundSource::setup()
	{
		alGenSources(1, &source_);
	}

	DesktopSoundSource::~DesktopSoundSource()
	{
		alDeleteSources(1, &source_);
	}

	void DesktopSoundSource::play(const DesktopSoundBuffer* buffer, const AudioData& sound, const void* owner)
	{
		owner_ = owner;

		alSourcef(source_, AL_PITCH, sound.getPitch());
		alSourcef(source_, AL_GAIN, sound.getGain());
		alSource3f(source_, AL_POSITION, sound.getPosition().x, sound.getPosition().y, sound.getPosition().z);
		alSource3f(source_, AL_VELOCITY, sound.getVelocity().x, sound.getVelocity().y, sound.getVelocity().z);
		alSourcei(source_, AL_LOOPING, sound.isLooping());

		if (buffer->getBuffer() != buffer_)
		{
			buffer_ = buffer->getBuffer();
			alSourcei(source_, AL_BUFFER, (ALint)buffer_);
		}
		alSourcePlay(source_);
	}
}