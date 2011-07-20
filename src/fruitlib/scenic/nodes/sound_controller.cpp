#include "sound_controller.hpp"
#include "../../audio/sound_controller.hpp"
#include <iostream>

fruitcut::fruitlib::scenic::nodes::sound_controller::sound_controller(
	audio::sound_controller &_sound_controller)
:
	sound_controller_(
		_sound_controller)
{
}

fruitcut::fruitlib::scenic::nodes::sound_controller::~sound_controller()
{
}

void
fruitcut::fruitlib::scenic::nodes::sound_controller::update()
{
	sound_controller_.update();
}

void
fruitcut::fruitlib::scenic::nodes::sound_controller::render()
{
}
