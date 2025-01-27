#pragma once
#include "BeaverEngine/Component/ManagerComponent.h"
#include "BeaverEngine/Utils/AudioManagerImpl.h"

namespace bv
{
    class AudioManagerComponent :
        public ManagerComponent
    {
    public:
        static constexpr auto type_ = "AudioManager";

        AudioManagerComponent(Entity& owner) : ManagerComponent(owner) { impl_ = AudioManagerImpl::create(); }
        void setup(const ComponentDescription& init_value) override {}
        void update(const Timing& dt) override { impl_->update(dt); }
        SoundPlayer* playSound(const Sound& sound, const void* owner) { return impl_->playSound(sound, owner); };
        void registerSound(const Sound* sound) { impl_->registerSound(sound); }
        void soundRemoved(const Sound* sound) { impl_->soundRemoved(sound); }
        void clean() { impl_->clean(); }
    private:
        std::unique_ptr<AudioManagerImpl> impl_;
    };
}

