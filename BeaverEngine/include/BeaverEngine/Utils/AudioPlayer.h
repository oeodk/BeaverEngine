#pragma once
#include "BeaverEngine/Enums/AudioState.h"
namespace bv
{
	class AudioPlayer
	{
	public:
		virtual void resume(const void* owner) const = 0;
		virtual void pause(const void* owner) const = 0;
		virtual void stop(const void* owner) = 0;
		virtual AudioState::AudioState getState() const = 0;

		bool isOwner(const void* owner)const { return owner_ == owner; }
	protected :
		const void* owner_{};
	};
}