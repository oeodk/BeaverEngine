#pragma once
namespace bv
{
	class SoundPlayer
	{
	public:
		enum SoundState
		{
			PLAYING, PAUSED, STOPPED
		};

		virtual void resume(const void* owner) const = 0;
		virtual void pause(const void* owner) const = 0;
		virtual SoundState getState() const = 0;

		bool isOwner(const void* owner)const { return owner_ == owner; }
	protected :
		const void* owner_{};
	};
}