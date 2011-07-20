#include "music_controller.hpp"
#include "../../audio/music_controller.hpp"
#include <iostream>

fruitcut::fruitlib::scenic::nodes::music_controller::music_controller(
	audio::music_controller &_music_controller)
:
	music_controller_(
		_music_controller)
{
}

fruitcut::fruitlib::scenic::nodes::music_controller::~music_controller()
{
}

void
fruitcut::fruitlib::scenic::nodes::music_controller::update()
{
	music_controller_.update();
}

void
fruitcut::fruitlib::scenic::nodes::music_controller::render()
{
}
