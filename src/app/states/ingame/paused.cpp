#include "paused.hpp"
#include "running.hpp"
#include "../../../fruitlib/time_format/string_to_duration_exn.hpp"
#include "../../../fruitlib/json/find_member.hpp"
#include "../../../fruitlib/pp/texture/use_screen_size.hpp"
#include "../../../fruitlib/pp/filter/blur.hpp"
#include "../../../media_path.hpp"
#include "../../postprocessing.hpp"
#include "../../events/generic_transition.hpp"
#include "../../scene.hpp"
#include <sge/renderer/device.hpp>
#include <sge/input/keyboard/action.hpp>
#include <sge/input/keyboard/key_code.hpp>
#include <sge/systems/instance.hpp>
#include <sge/time/time.hpp>
#include <fcppt/text.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/tr1/functional.hpp>
#include <fcppt/assign/make_container.hpp>
#include <iostream>

fruitcut::app::states::ingame::paused::paused(
	my_context ctx)
:
	my_base(
		ctx),
	time_factor_(
		context<machine>(),
		static_cast<sge::time::funit>(
			0)),
	scene_deactivation_(
		context<machine>().scene_node(),
		false),
	system_(
		context<machine>().postprocessing().filter_manager()),
	inject_texture_(
		context<machine>().postprocessing().texture_manager()),
	blur_(
		context<machine>().systems().renderer(),
		context<machine>().postprocessing().filter_manager(),
		context<machine>().postprocessing().texture_manager(),
		fruitlib::pp::texture::use_screen_size(),
		static_cast<fruitlib::pp::filter::blur::size_type>(
			1)),
	current_texture_(
		context<machine>().postprocessing().result_texture()),
	blur_iterations_(
		static_cast<fruitlib::pp::filter::blur::size_type>(
			0)),
	max_blur_iterations_(
		fruitlib::json::find_member<fruitlib::pp::filter::blur::size_type>(
			context<machine>().config_file(),
			FCPPT_TEXT("paused/max-blur-iterations"))),
	blur_timer_(
		fruitlib::time_format::string_to_duration_exn<sge::time::duration>(
			fruitlib::json::find_member<fcppt::string>(
				context<machine>().config_file(),
				FCPPT_TEXT("paused/blur-frequency-time")))),
	transit_to_running_connection_(
		context<machine>().game_input_state().key_callback(
			sge::input::keyboard::action(
				sge::input::keyboard::key_code::p, 
				std::tr1::bind(
					// Note that using post_event does something unexpected. If
					// you use that, you get a tick event first and _then_ the
					// toggle_pause event, which is not the desired behaviour
					// (post_event posts to the queue, process_event immediately
					// processes it)
					&machine::post_event,
					&context<machine>(),
					events::generic_transition<ingame::running>()))))
{
	context<machine>().root_node().insert_after(
		*this,
		context<machine>().scene_node());

	system_.add_filter(
		inject_texture_,
		FCPPT_TEXT("inject_texture"),
		fruitlib::pp::dependency_set());

	system_.add_filter(
		blur_,
		FCPPT_TEXT("blur"),
		fcppt::assign::make_container<fruitlib::pp::dependency_set>
			(FCPPT_TEXT("inject_texture")));
}

fruitcut::app::states::ingame::paused::~paused()
{
}

void
fruitcut::app::states::ingame::paused::render()
{
	system_.render_result();
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

