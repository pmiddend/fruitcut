#include <fruitlib/scenic/adaptors/gui_system.hpp>
#include <fruitlib/scenic/events/render.hpp>
#include <fruitlib/scenic/events/update.hpp>
#include <sge/cegui/duration.hpp>
#include <sge/cegui/system.hpp>
#include <sge/timer/elapsed_and_reset.hpp>
#include <sge/timer/parameters.hpp>
#include <fcppt/config/external_begin.hpp>
#include <chrono>
#include <fcppt/config/external_end.hpp>


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
			std::chrono::seconds(1))),
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
	fruitlib::scenic::events::render const &_render_event)
{
	system_.render(
		_render_event.context());
}
