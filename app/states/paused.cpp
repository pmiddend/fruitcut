#include "paused.hpp"
#include "running.hpp"
#include <sge/renderer/device.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/assign/make_container.hpp>
#include <iostream>

fruitcut::app::states::paused::paused(
	my_context ctx)
:
	my_base(
		ctx),
	system_(
		context<machine>().systems().renderer()),
	inject_texture_(
		context<machine>().postprocessing().texture_manager()),
	blur_(
		context<machine>().systems().renderer(),
		context<machine>().postprocessing().texture_manager(),
		fcppt::math::dim::structure_cast<sge::renderer::dim2>(
			context<machine>().systems().renderer()->screen_size()),
		static_cast<pp::filter::blur::size_type>(
			1)),
	current_texture_(
		context<machine>().postprocessing().result_texture())
{
	context<machine>().postprocessing().active(
		false);

	system_.add_filter(
		inject_texture_,
		FCPPT_TEXT("inject_texture"),
		fruitcut::pp::dependency_set());

	system_.add_filter(
		blur_,
		FCPPT_TEXT("blur"),
		fcppt::assign::make_container<fruitcut::pp::dependency_set>
			(FCPPT_TEXT("inject_texture")));
}

boost::statechart::result
fruitcut::app::states::paused::react(
	events::render_overlay const &)
{
	context<machine>().particle_system().render();
	system_.render_result();
	return discard_event();
}

boost::statechart::result
fruitcut::app::states::paused::react(
	events::tick const &)
{
	context<machine>().particle_system().update();
	static unsigned its = 0;
	if (++its < 10)
	{
		inject_texture_.texture(
			current_texture_);
		system_.update();
		current_texture_ = 
			system_.result_texture();
	}
	return discard_event();
}

boost::statechart::result
fruitcut::app::states::paused::react(
	events::toggle_pause const &)
{
	return transit<running>();
}

fruitcut::app::states::paused::~paused()
{
}
