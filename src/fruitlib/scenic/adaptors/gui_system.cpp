#include <fruitlib/scenic/adaptors/gui_system.hpp>
#include <fruitlib/scenic/events/update.hpp>
#include <fruitlib/scenic/events/render.hpp>
#include <sge/cegui/system.hpp>
#include <sge/cegui/duration.hpp>
#include <sge/timer/parameters.hpp>
#include <sge/timer/elapsed_and_reset.hpp>
#include <fcppt/chrono/seconds.hpp>
#include <fcppt/chrono/duration_cast.hpp>

fruitlib::scenic::adaptors::gui_system::gui_system(
	scenic::optional_parent const &_parent,
	sge::cegui::system &_system,
	scenic::delta::callback const &_callback)
:
	node_base(
		_parent),
	clock_(
		_callback),
	timer_(
		scenic::delta::timer::parameters(
			clock_,
			fcppt::chrono::seconds(1))),
	system_(
		_system)
{
}

fruitlib::scenic::adaptors::gui_system::~gui_system()
{
}

void
fruitlib::scenic::adaptors::gui_system::react(
	events::update const &)
{
	clock_.update();
	system_.update(
		sge::timer::elapsed_and_reset<sge::cegui::duration>(
			timer_));
}

void
fruitlib::scenic::adaptors::gui_system::react(
	events::render const &)
{
	system_.render();
}
