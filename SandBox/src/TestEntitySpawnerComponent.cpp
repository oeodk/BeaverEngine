#include "TestEntitySpawnerComponent.h"
#include "BeaverEngine/BeaverEngine.h"
#include "yaml-cpp/yaml.h"
#include <BeaverEngine/Core/GlobalConstants.h>
#include <BeaverEngine/System/AudioSystem.h>
#include <BeaverEngine/System/InputSystem.h>
#include <BeaverEngine/System/WindowSystem.h>
#include <BeaverEngine/Utils/Window.h>
#include <BeaverEngine/Core/Game.h>

void sandbox::TestEntitySpawnerComponent::setup(const bv::ComponentDescription& init_value)
{
	component_template_ = bv::Descr::load(init_value.parameters.at("path").as<std::string>());
	sound_.setup(bv::constants::SOUNDS_PATH + "player_hit.wav");
	music_.setup(bv::constants::MUSICS_PATH + "boss_room.wav");
}

void sandbox::TestEntitySpawnerComponent::resolve()
{
	for (int i = 0; i < 0; i++)
	{
		component_template_[0]["name"] = "TestComponent" + std::to_string(i);
		component_template_[0]["description"]["components"]["Position"]["x"] = rand() % 400 - 200;
		component_template_[0]["description"]["components"]["Position"]["y"] = rand() % 400 - 200;

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
	if (bv::InputSystem::getInstance().isKeyPressed(bv::Key::ESCAPE))
		bv::Game::close();

	if (bv::InputSystem::getInstance().isKeyHeld(bv::Key::RIGHT))
		bv::WindowSystem::getInstance().getMainWindow().lock()->move({ 10,0 });
	if (bv::InputSystem::getInstance().isKeyHeld(bv::Key::LEFT))
		bv::WindowSystem::getInstance().getMainWindow().lock()->move({ -10,0 });
	if (bv::InputSystem::getInstance().isKeyHeld(bv::Key::UP))
		bv::WindowSystem::getInstance().getMainWindow().lock()->move({ 0,10 });
	if (bv::InputSystem::getInstance().isKeyHeld(bv::Key::DOWN))
		bv::WindowSystem::getInstance().getMainWindow().lock()->move({ 0,-10 });

	if (bv::InputSystem::getInstance().isKeyHeld(bv::Key::ENTER))
		bv::WindowSystem::getInstance().getMainWindow().lock()->focus();

}
