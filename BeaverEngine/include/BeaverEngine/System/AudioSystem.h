#pragma once
#include "BeaverEngine/System/System.h"
#include "BeaverEngine/System/AudioSystemImpl.h"

namespace bv
{
    class AudioSystem :
        public System
    {
    public:
        AudioSystem() { impl_ = AudioSystemImpl::create(); }
        AudioSystem(const AudioSystem&) = default;
        AudioSystem(AudioSystem&&) = default;
        AudioSystem& operator=(const AudioSystem&) = default;
        AudioSystem& operator=(AudioSystem&&) = default;

        virtual ~AudioSystem() = default;

        static AudioSystem& getInstance();

        void iterate(const Timing& dt) override { impl_->iterate(dt); }
        SoundPlayer* playSound(const Sound& sound, const void* owner) { return impl_->playSound(sound, owner); };
        void registerSound(const Sound* sound) { impl_->registerSound(sound); }
        void soundRemoved(const Sound* sound) { impl_->soundRemoved(sound); }
        void clean() { impl_->clean(); }
    private:
        std::unique_ptr<AudioSystemImpl> impl_;
    };
}

