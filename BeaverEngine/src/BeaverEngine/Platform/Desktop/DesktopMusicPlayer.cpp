#include "BeaverEngine/Platform/Desktop/DesktopMusicPlayer.h"
#include "BeaverEngine/Utils/AudioData.h"
#include <AL\alext.h>

namespace bv
{
	void DesktopMusicPlayer::setup()
	{
		alGenSources(1, &source_);
		alGenBuffers(NUM_BUFFERS, buffers_);
	}

	DesktopMusicPlayer::~DesktopMusicPlayer()
	{
		alSourceStop(source_);
		alDeleteSources(1, &source_);

		if (sndFile_)
			sf_close(sndFile_);

		sndFile_ = nullptr;

		free(membuf_);

		alDeleteBuffers(NUM_BUFFERS, buffers_);
	}

	void DesktopMusicPlayer::update()
	{
		ALint processed, state;

		ALint looping = 0;
		alGetSourcei(source_, AL_LOOPING, &looping);

		alSourcei(source_, AL_LOOPING, AL_FALSE);

		// clear error 
		alGetError();
		/* Get relevant source info */
		alGetSourcei(source_, AL_SOURCE_STATE, &state);
		alGetSourcei(source_, AL_BUFFERS_PROCESSED, &processed);
		if (alGetError() != AL_NO_ERROR)
		{
			throw("error checking music source state");
		}

		/* Unqueue and handle each processed buffer */
		while (processed > 0)
		{
			ALuint bufid;
			sf_count_t slen;

			alSourceUnqueueBuffers(source_, 1, &bufid);
			processed--;

			/* Read the next chunk of data, refill the buffer, and queue it
			 * back on the source */
			slen = sf_readf_short(sndFile_, membuf_, BUFFER_SAMPLES);

			

			if (slen == 0 && looping == AL_TRUE)  // End of file, restart from the beginning
			{
				sf_seek(sndFile_, 0, SEEK_SET);
				slen = sf_readf_short(sndFile_, membuf_, BUFFER_SAMPLES);
			}

			if (slen > 0)
			{
				slen *= sfinfo_.channels * (sf_count_t)sizeof(short);
				alBufferData(bufid, format_, membuf_, (ALsizei)slen,
					sfinfo_.samplerate);
				alSourceQueueBuffers(source_, 1, &bufid);
			}
			if (alGetError() != AL_NO_ERROR)
			{
				throw("error buffering music data");
			}
		}

		/* Make sure the source hasn't underrun */
		if (state != AL_PLAYING && state != AL_PAUSED)
		{
			ALint queued;

			/* If no buffers are queued, playback is finished */
			alGetSourcei(source_, AL_BUFFERS_QUEUED, &queued);
			if (queued == 0)
				return;

			alSourcePlay(source_);
			if (alGetError() != AL_NO_ERROR)
			{
				throw("error restarting music playback");
			}
		}

		alSourcei(source_, AL_LOOPING, looping);

	}

	void DesktopMusicPlayer::play(const AudioData& sound, const void* owner)
	{
		owner_ = owner;
		keep_playing = true;
		alSourcef(source_, AL_PITCH, sound.getPitch());
		alSourcef(source_, AL_GAIN, sound.getGain());
		alSource3f(source_, AL_POSITION, sound.getPosition().x, sound.getPosition().y, sound.getPosition().z);
		alSource3f(source_, AL_VELOCITY, sound.getVelocity().x, sound.getVelocity().y, sound.getVelocity().z);
		alSourcei(source_, AL_LOOPING, sound.isLooping());

		loadFile(sound.getPath());
		startMusic();
	}
	void DesktopMusicPlayer::loadFile(const std::string& path)
	{
		std::size_t frame_size;

		sndFile_ = sf_open(path.c_str(), SFM_READ, &sfinfo_);
		if (!sndFile_)
		{
			throw("could not open provided music file -- check path");
		}

		/* Get the sound format, and figure out the OpenAL format */
		if (sfinfo_.channels == 1)
			format_ = AL_FORMAT_MONO16;
		else if (sfinfo_.channels == 2)
			format_ = AL_FORMAT_STEREO16;
		else if (sfinfo_.channels == 3)
		{
			if (sf_command(sndFile_, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
				format_ = AL_FORMAT_BFORMAT2D_16;
		}
		else if (sfinfo_.channels == 4)
		{
			if (sf_command(sndFile_, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
				format_ = AL_FORMAT_BFORMAT3D_16;
		}
		if (!format_)
		{
			sf_close(sndFile_);
			sndFile_ = NULL;
			throw("Unsupported channel count from file");
		}

		frame_size = ((size_t)BUFFER_SAMPLES * (size_t)sfinfo_.channels) * sizeof(short);
		membuf_ = static_cast<short*>(malloc(frame_size));
	}
	void DesktopMusicPlayer::startMusic()
	{
		ALsizei i;

		// clear any al errors
		alGetError();

		/* Rewind the source position and clear the buffer queue */
		alSourceRewind(source_);
		alSourcei(source_, AL_BUFFER, 0);

		/* Fill the buffer queue */
		for (i = 0; i < NUM_BUFFERS; i++)
		{
			/* Get some data to give it to the buffer */
			sf_count_t slen = sf_readf_short(sndFile_, membuf_, BUFFER_SAMPLES);
			if (slen < 1) break;

			slen *= sfinfo_.channels * (sf_count_t)sizeof(short);
			alBufferData(buffers_[i], format_, membuf_, (ALsizei)slen, sfinfo_.samplerate);
		}
		if (alGetError() != AL_NO_ERROR)
		{
			throw("Error buffering for playback");
		}

		/* Now queue and start playback! */
		alSourceQueueBuffers(source_, i, buffers_);
		alSourcePlay(source_);
		if (alGetError() != AL_NO_ERROR)
		{
			throw("Error starting playback");
		}
	}
}
