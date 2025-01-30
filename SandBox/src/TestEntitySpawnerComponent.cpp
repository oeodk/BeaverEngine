#include "TestEntitySpawnerComponent.h"
#include "BeaverEngine/BeaverEngine.h"
#include "yaml-cpp/yaml.h"
#include <BeaverEngine/Core/GlobalConstants.h>
#include <BeaverEngine/System/AudioSystem.h>
#include <BeaverEngine/System/InputSystem.h>

void sandbox::TestEntitySpawnerComponent::setup(const bv::ComponentDescription& init_value)
{
	component_template_ = bv::Descr::load(init_value.parameters.at("path").as<std::string>());
	sound_.setup(bv::constants::SOUNDS_PATH + "player_hit.wav");
	music_.setup(bv::constants::MUSICS_PATH + "boss_room.wav");
}

void sandbox::TestEntitySpawnerComponent::resolve()
{
	for (int i = 0; i < 10; i++)
	{
		component_template_[0]["name"] = "TestComponent" + std::to_string(i);
		component_template_[0]["description"]["components"][0]["param"][0]["value"] = rand() % 400 - 200;
		component_template_[0]["description"]["components"][0]["param"][1]["value"] = rand() % 400 - 200;

		bv::Scene::CreateChild(component_template_[0], owner().shared_from_this());
	}
	player_ = bv::AudioSystem::getInstance().playSound(sound_, this);
	bv::AudioSystem::getInstance().playMusic(music_, this);
}

void sandbox::TestEntitySpawnerComponent::updateLogic(const bv::Timing& dt)
{
	if (bv::InputSystem::getInstance().isKeyPressed(bv::Key::SPACE))
	{
		switch (player_->getState())
		{
		case bv::AudioState::PLAYING:
			player_->pause(this);
			break;
		case bv::AudioState::PAUSED:
			player_->resume(this);
			break;
		case bv::AudioState::STOPPED:
			player_ = bv::AudioSystem::getInstance().playSound(sound_, this);
			break;
		default:
			break;
		}
	}
}
