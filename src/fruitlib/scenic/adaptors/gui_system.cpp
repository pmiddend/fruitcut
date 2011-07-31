#include <fruitlib/scenic/adaptors/gui_system.hpp>
#include <fruitlib/scenic/events/update.hpp>
#include <fruitlib/scenic/events/render.hpp>
#include <sge/cegui/system.hpp>
#include <sge/cegui/duration.hpp>
#include <fcppt/chrono/duration_cast.hpp>

fruitlib::scenic::adaptors::gui_system::gui_system(
	scenic::optional_parent const &_parent,
	sge::cegui::system &_system)
:
	node_base(
		_parent),
	system_(
		_system)
{
}

fruitlib::scenic::adaptors::gui_system::~gui_system()
{
}

void
fruitlib::scenic::adaptors::gui_system::react(
	events::update const &d)
{
	system_.update(
		fcppt::chrono::duration_cast<sge::cegui::duration>(
			d.delta()));
}

void
fruitlib::scenic::adaptors::gui_system::react(
	events::render const &)
{
	system_.render();
}
