#include "gui_system.hpp"
#include <sge/cegui/system.hpp>
#include <sge/time/second.hpp>
#include <sge/time/second_f.hpp>

fruitcut::scenic::nodes::gui_system::gui_system(
	sge::cegui::system &_system,
	sge::time::callback const &_time_callback)
:
	frame_timer_(
		sge::time::second(1),
		sge::time::activation_state::active,
		_time_callback),
	system_(
		_system)
{
}

fruitcut::scenic::nodes::gui_system::~gui_system()
{
}

void
fruitcut::scenic::nodes::gui_system::update()
{
	system_.update(
		sge::time::second_f(
			frame_timer_.reset()));
}

void
fruitcut::scenic::nodes::gui_system::render()
{
	system_.render();
}
