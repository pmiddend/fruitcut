#include "sound_controller.hpp"
#include "../../audio/sound_controller.hpp"

fruitcut::scenic::nodes::sound_controller::sound_controller(
	audio::sound_controller &_sound_controller)
:
	sound_controller_(
		_sound_controller)
{
}

fruitcut::scenic::nodes::sound_controller::~sound_controller()
{
}

void
fruitcut::scenic::nodes::sound_controller::update()
{
	sound_controller_.update();
}

void
fruitcut::scenic::nodes::sound_controller::render()
{
}
