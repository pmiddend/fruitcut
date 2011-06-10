#include "intro.hpp"
#include "ingame/running.hpp"
#include "../../fruitlib/time_format/string_to_duration_exn.hpp"
#include "../../fruitlib/json/find_member.hpp"
#include "../../fruitlib/resource_tree/path.hpp"
#include <sge/time/time.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/viewport/manager.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/state/state.hpp>
#include <sge/renderer/onscreen_target.hpp>
#include <fcppt/text.hpp>
#include <fcppt/tr1/functional.hpp>
#include <fcppt/string.hpp>

fruitcut::app::states::intro::intro(
	my_context ctx)
:
	my_base(
		ctx),
	scoped_render_state_(
		context<machine>().systems().renderer(),
		sge::renderer::state::list
			(sge::renderer::state::depth_func::less)
			(sge::renderer::state::cull_mode::off)
			(sge::renderer::state::bool_::clear_depth_buffer = true)
			(sge::renderer::state::float_::depth_buffer_clear_val = 1.0f)),
	// Those timers will be activated as soon as we have a viewport
	saturation_timer_(
		fruitlib::time_format::string_to_duration_exn<sge::time::duration>(
			fruitlib::json::find_member<fcppt::string>(
				context<machine>().config_file(),
				FCPPT_TEXT("intro/desaturation-time"))),
		sge::time::activation_state::inactive,
		context<machine>().timer_callback()),
	intro_timer_(
		fruitlib::time_format::string_to_duration_exn<sge::time::duration>(
			fruitlib::json::find_member<fcppt::string>(
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
	context<machine>().sound_controller().play(
		fruitlib::resource_tree::path(
			FCPPT_TEXT("intro")));

	// We already have a viewport? Ok, then go
	if(context<machine>().systems().renderer().onscreen_target().viewport().get().size().content())
		viewport_change();

	context<machine>().overlay_node().insert_dont_care(
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
}
