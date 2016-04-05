#include <fruitapp/postprocessing/cg/system.hpp>
#include <fruitapp/postprocessing/cg/subsystems/main.hpp>
#include <fruitapp/postprocessing/cg/subsystems/paused.hpp>
#include <fruitlib/pp/texture/use_screen_size.hpp>
#include <fruitlib/time_format/find_and_convert_duration.hpp>
#include <sge/parse/json/find_and_convert_member.hpp>
#include <sge/parse/json/path.hpp>
#include <sge/renderer/context/ffp.hpp>
#include <sge/timer/parameters.hpp>
#include <sge/timer/reset_when_expired.hpp>
#include <fcppt/text.hpp>


fruitapp::postprocessing::cg::subsystems::paused::paused(
	fruitapp::postprocessing::cg::system &_parent_system,
	fruitlib::scenic::optional_parent const &_parent)
:
	node_base(
		_parent),
	parent_system_(
		_parent_system),
	system_(
		_parent_system.filter_manager_),
	inject_texture_(),
	blur_(
		_parent_system.filter_manager_,
		_parent_system.texture_manager_,
		fruitlib::pp::filter::texture_size(
			fruitlib::pp::texture::use_screen_size()),
		fruitlib::pp::filter::iterations(
			1u)),
	current_texture_(
		_parent_system.main_result_texture()),
	blur_iterations_(
		0u),
	max_blur_iterations_(
		sge::parse::json::find_and_convert_member<fruitlib::pp::filter::iterations::value_type>(
			_parent_system.configuration_,
			sge::parse::json::path(FCPPT_TEXT("paused"))
				/ FCPPT_TEXT("max-blur-iterations"))),
	blur_timer_(
		sge::timer::parameters<sge::timer::clocks::standard>(
			fruitlib::time_format::find_and_convert_duration<sge::timer::clocks::standard::duration>(
				_parent_system.configuration_,
				sge::parse::json::path(FCPPT_TEXT("paused")) / FCPPT_TEXT("blur-frequency-time"))))
{
	system_.add_filter(
		inject_texture_,
		FCPPT_TEXT("inject_texture"),
		fruitlib::pp::dependency_set());

	system_.add_filter(
		blur_,
		FCPPT_TEXT("blur"),
		fruitlib::pp::dependency_set{
			FCPPT_TEXT("inject_texture")
		}
	);
}

void
fruitapp::postprocessing::cg::subsystems::paused::react(
	fruitlib::scenic::events::render const &_render_event)
{
	system_.render_result(
		_render_event.context());
}

void
fruitapp::postprocessing::cg::subsystems::paused::react(
	fruitlib::scenic::events::update const &)
{
	if(
		!blur_iterations_.get() ||
		(blur_iterations_ < max_blur_iterations_ && sge::timer::reset_when_expired(blur_timer_)))
	{
		inject_texture_.texture(
			current_texture_);
		system_.update();
		current_texture_ =
			system_.result_texture();
		blur_iterations_++;
	}
}

fruitapp::postprocessing::cg::subsystems::paused::~paused()
{
	parent_system_.paused_destroyed();
}
