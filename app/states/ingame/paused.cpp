#include "paused.hpp"
#include "running.hpp"
#include "../../../string_to_duration_exn.hpp"
#include "../../../json/find_member.hpp"
#include "../../../pp/texture/use_screen_size.hpp"
#include "../../../pp/filter/blur.hpp"
#include <sge/renderer/device.hpp>
#include <sge/time/second_f.hpp>
#include <sge/time/activation_state.hpp>
#include <sge/time/funit.hpp>
#include <fcppt/text.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/assign/make_container.hpp>
#include <iostream>

fruitcut::app::states::ingame::paused::paused(
	my_context ctx)
:
	my_base(
		ctx),
	scene_deactivation_(
		context<machine>().scene_node(),
		false),
	system_(
		context<machine>().systems().renderer()),
	inject_texture_(
		context<machine>().postprocessing().texture_manager()),
	blur_(
		context<machine>().systems().renderer(),
		context<machine>().postprocessing().texture_manager(),
		pp::texture::use_screen_size(),
		static_cast<pp::filter::blur::size_type>(
			1)),
	current_texture_(
		context<machine>().postprocessing().result_texture()),
	blur_iterations_(
		static_cast<pp::filter::blur::size_type>(
			0)),
	max_blur_iterations_(
		json::find_member<pp::filter::blur::size_type>(
			context<machine>().config_file(),
			FCPPT_TEXT("paused/max-blur-iterations"))),
	blur_timer_(
		string_to_duration_exn<sge::time::duration>(
			json::find_member<fcppt::string>(
				context<machine>().config_file(),
				FCPPT_TEXT("paused/blur-frequency-time"))),
		sge::time::activation_state::active,
		context<machine>().timer_callback())
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

void
fruitcut::app::states::ingame::paused::render()
{
	system_.render_result();
}

boost::statechart::result
fruitcut::app::states::ingame::paused::react(
	events::toggle_pause const &)
{
	return transit<running>();
}

fruitcut::app::states::ingame::paused::~paused()
{
}

void
fruitcut::app::states::ingame::paused::update()
{
	if(!blur_iterations_ || (blur_iterations_ < max_blur_iterations_ && blur_timer_.update_b()))
	{
		inject_texture_.texture(
			current_texture_);
		system_.update();
		current_texture_ = 
			system_.result_texture();
		blur_iterations_++;
	}
}

