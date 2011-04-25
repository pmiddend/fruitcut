#include "music_controller.hpp"
#include "../../audio/music_controller.hpp"

fruitcut::scenic::nodes::music_controller::music_controller(
	audio::music_controller &_music_controller)
:
	music_controller_(
		_music_controller)
{
}

fruitcut::scenic::nodes::music_controller::~music_controller()
{
}

void
fruitcut::scenic::nodes::music_controller::update()
{
	music_controller_.update();
}

void
fruitcut::scenic::nodes::music_controller::render()
{
}
