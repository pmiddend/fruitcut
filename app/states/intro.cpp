#include "intro.hpp"
#include "ingame/running.hpp"
#include "../string_to_duration_exn.hpp"
#include "../json/find_member.hpp"
#include <sge/time/time.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/viewport/manager.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/onscreen_target.hpp>
#include <fcppt/text.hpp>
#include <fcppt/tr1/functional.hpp>
#include <fcppt/string.hpp>

fruitcut::app::states::intro::intro(
	my_context ctx)
:
	my_base(
		ctx),
	// Those timers will be activated as soon as we have a viewport
	saturation_timer_(
		string_to_duration_exn<sge::time::duration>(
			json::find_member<fcppt::string>(
				context<machine>().config_file(),
				FCPPT_TEXT("intro/desaturation-time"))),
		sge::time::activation_state::inactive,
		context<machine>().timer_callback()),
	intro_timer_(
		string_to_duration_exn<sge::time::duration>(
			json::find_member<fcppt::string>(
				context<machine>().config_file(),
				FCPPT_TEXT("intro/total-duration"))),
		sge::time::activation_state::inactive,
		context<machine>().timer_callback()),
	viewport_change_connection_(
		context<machine>().systems().viewport_manager().manage_callback(
			std::tr1::bind(
				&intro::viewport_change,
				this))),
	logo_(
		context<machine>().systems().renderer(),
		context<machine>().systems().viewport_manager(),
		context<machine>().systems().image_loader(),
		context<machine>().config_file())
{
	// We already have a viewport? Ok, then go
	if(context<machine>().systems().renderer().onscreen_target().viewport().get().size().content())
		viewport_change();

	context<machine>().overlay_node().children().push_back(
		logo_);
}

boost::statechart::result
fruitcut::app::states::intro::react(
	events::render const &)
{
	return discard_event();
}

boost::statechart::result
fruitcut::app::states::intro::react(
	events::tick const &)
{
	context<machine>().postprocessing().desaturate_filter().factor(
		static_cast<sge::renderer::scalar>(
			saturation_timer_.expired()
			?
				static_cast<sge::renderer::scalar>(1.0)
			:
				saturation_timer_.elapsed_frames()));
	if (intro_timer_.expired())
		return transit<ingame::running>();
	return discard_event();
}

fruitcut::app::states::intro::~intro()
{
}

void
fruitcut::app::states::intro::viewport_change()
{
	if (saturation_timer_.active())
		return;

	saturation_timer_.activate();
	intro_timer_.activate();

	context<machine>().sound_controller().play(
		FCPPT_TEXT("intro"));

}
