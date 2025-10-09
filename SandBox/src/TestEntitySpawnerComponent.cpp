#include "TestEntitySpawnerComponent.h"
#include "BeaverEngine/BeaverEngine.h"
#include "yaml-cpp/yaml.h"
#include <BeaverEngine/Core/GlobalConstants.h>
#include <BeaverEngine/System/AudioSystem.h>
#include <BeaverEngine/System/InputSystem.h>
#include <BeaverEngine/System/WindowSystem.h>
#include <BeaverEngine/Utils/Window.h>
#include <BeaverEngine/Core/Game.h>
#include <iostream>

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
		component_template_[0]["description"]["components"]["Position"]["x"] = rand() % 400 - 200;
		component_template_[0]["description"]["components"]["Position"]["y"] = rand() % 400 - 200;

		bv::Scene::CreateChild(component_template_[0], owner().shared_from_this());
	}
	player_ = bv::AudioSystem::getInstance().playSound(sound_, this);
	bv::AudioSystem::getInstance().playMusic(music_, this);

	bv::ParticleSystem::getInstance().initializeView(bv::WindowSystem::getInstance().getWindow("second_window").lock().get(), bv::ViewSystem::getInstance().getView("second_view"));
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

	//printf("x : %f, y : %f\n", bv::InputSystem::getInstance().getJoystickAxisValue(bv::Axis::LEFT_X), bv::InputSystem::getInstance().getJoystickAxisValue(bv::Axis::LEFT_Y));
	//printf("x : %d, y : %d\n", bv::InputSystem::getInstance().getJoystickAxisDirection(bv::Axis::LEFT_X), bv::InputSystem::getInstance().getJoystickAxisDirection(bv::Axis::LEFT_Y));

	auto window = bv::WindowSystem::getInstance().getMainWindow().lock();
	if (bv::InputSystem::getInstance().getJoystickAxisDirection(bv::Axis::LEFT_X) == 1)
		window->setSize(window->getWidth() + 10, window->getHeight());
	if (bv::InputSystem::getInstance().getJoystickAxisDirection(bv::Axis::LEFT_X) == -1)
		window->setSize(window->getWidth() - 10, window->getHeight());
	if (bv::InputSystem::getInstance().getJoystickAxisDirection(bv::Axis::LEFT_Y) == 1)
		window->setSize(window->getWidth(), window->getHeight() + 10);
	if (bv::InputSystem::getInstance().getJoystickAxisDirection(bv::Axis::LEFT_Y) == -1)
		window->setSize(window->getWidth(), window->getHeight() - 10);

	//std::cout << bv::InputSystem::getInstance().getJoystickAxisValue(bv::Axis::RIGHT_X) << std::endl;
	if (bv::InputSystem::getInstance().isMouseButtonHeld(bv::Mouse::LEFT))
	{
		glm::vec2 pos = bv::ViewSystem::getInstance().getMainView()->mapPixelToCoords({ bv::InputSystem::getInstance().getScreenMousePosition().x , bv::InputSystem::getInstance().getScreenMousePosition().y });
		bv::ParticleProps props;
		props.position = { pos, 10 };
		props.start_size = { 25,25 };
		props.end_size = { 5, 5 };
		props.lifespan_s = 1;
		props.lifespan_variation = 0.5f;

		props.velocity_variation = { 5,5,0 };

		props.start_color = { 1,0,0,1 };
		props.end_color = { 0,0,1,0 };

		props.rotation_angle = bv::Random::randomFloat() * 360;

		for(int i = 0;i<50;i++)
		{
			props.velocity = 100.f * glm::vec3{ bv::Random::randomFloat() - 0.5, bv::Random::randomFloat() - 0.5 ,0 };
			bv::ParticleSystem::getInstance().emitParticle(props);
		}
		//std::cout << pos.x << " | " << pos.y << std::endl;
	}
	

	if (bv::InputSystem::getInstance().isControllerButtonPressed(bv::Gamepad::A))
	{ 
		bv::InputSystem::getInstance().makeControllerRumble(bv::Joystick::JOYSTICK_1, 0x0000, 0x7777, 1000);
	}
	if (bv::InputSystem::getInstance().isControllerButtonReleased(bv::Gamepad::A))
		bv::InputSystem::getInstance().makeControllerRumble(bv::Joystick::JOYSTICK_1, 0xFFFF, 0x0000, 100);
}
