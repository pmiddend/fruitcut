#include "gui_system.hpp"
#include "../events/update.hpp"
#include "../events/render.hpp"
#include <sge/cegui/system.hpp>
#include <sge/cegui/duration.hpp>
#include <fcppt/chrono/duration_cast.hpp>

fruitcut::fruitlib::scenic::adaptors::gui_system::gui_system(
	scenic::parent const &_parent,
	sge::cegui::system &_system)
:
	node_base(
		_parent),
	system_(
		_system)
{
}

fruitcut::fruitlib::scenic::adaptors::gui_system::~gui_system()
{
}

void
fruitcut::fruitlib::scenic::adaptors::gui_system::react(
	events::update const &d)
{
	system_.update(
		fcppt::chrono::duration_cast<sge::cegui::duration>(
			d.delta()));
}

void
fruitcut::fruitlib::scenic::adaptors::gui_system::react(
	events::render const &)
{
	system_.render();
}
