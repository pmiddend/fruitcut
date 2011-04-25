#include "cegui.hpp"
#include <sge/cegui/system.hpp>
#include <sge/time/second.hpp>
#include <sge/time/second_f.hpp>
#include <sge/time/activation_state.hpp>

fruitcut::scenic::nodes::cegui::cegui(
	sge::cegui::system &_system,
	sge::time::callback const &_callback)
:
	frame_timer_(
		sge::time::second(1),
		sge::time::activation_state::active,
		_callback),
	system_(
		_system)
{
}

fruitcut::scenic::nodes::cegui::~cegui()
{
}

void
fruitcut::scenic::nodes::cegui::update()
{
	system_.update(
		sge::time::second_f(
			frame_timer_.elapsed_frames()));
}

void
fruitcut::scenic::nodes::cegui::render()
{
	system_.render();
}
