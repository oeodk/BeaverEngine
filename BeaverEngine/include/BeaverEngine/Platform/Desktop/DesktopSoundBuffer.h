#pragma once
#include "BeaverEngine/Core/std.h"
#include <AL/al.h>
namespace bv
{
	class DesktopSoundBuffer
	{
	public:
		DesktopSoundBuffer(const std::string& path);
		~DesktopSoundBuffer();
		DesktopSoundBuffer(DesktopSoundBuffer&&) = default;
		DesktopSoundBuffer& operator=(const DesktopSoundBuffer&) = default;
		DesktopSoundBuffer& operator=(DesktopSoundBuffer&&) = default;

		DesktopSoundBuffer(const DesktopSoundBuffer&) = delete;
		const ALuint getBuffer() const { return buffer_; }
	private:

		ALuint buffer_;
	};
}

