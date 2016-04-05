#include <fruitapp/postprocessing/cg/system.hpp>
#include <fruitapp/postprocessing/cg/subsystems/main.hpp>
#include <fruitlib/pp/texture/use_screen_size.hpp>
#include <sge/image/color/predef.hpp>
#include <sge/parse/json/find_and_convert_member.hpp>
#include <sge/parse/json/string_to_path.hpp>
#include <sge/renderer/clear/parameters.hpp>
#include <fcppt/text.hpp>


fruitapp::postprocessing::cg::subsystems::main::main(
	fruitapp::postprocessing::cg::system &_parent_system,
	fruitlib::scenic::optional_parent const &_parent,
	fruitapp::postprocessing::render_callback const &_render_callback)
:
	node_base(
		_parent),
	parent_system_(
		_parent_system),
	active_(
		true),
	system_(
		_parent_system.filter_manager_),
	rtt_filter_(
		_parent_system.filter_manager_,
		_parent_system.texture_manager_,
		fruitlib::pp::filter::texture_size(
			fruitlib::pp::texture::use_screen_size()),
		sge::renderer::clear::parameters()
			.back_buffer(
				sge::renderer::clear::back_buffer_value(
					sge::image::color::predef::black()))
			.depth_buffer(
				sge::renderer::clear::depth_buffer_value(
					1.0f)),
		_render_callback,
		fruitlib::pp::texture::depth_stencil_format::d32),
	highlight_filter_(
		_parent_system.filter_manager_,
		_parent_system.texture_manager_,
		fruitlib::pp::filter::texture_size(
			sge::parse::json::find_and_convert_member<sge::renderer::dim2>(
				_parent_system.configuration_,
				sge::parse::json::string_to_path(
					FCPPT_TEXT("bloom-size")))),
		fruitlib::pp::filter::highlight::threshold_factor(
			sge::parse::json::find_and_convert_member<sge::renderer::scalar>(
				_parent_system.configuration_,
				sge::parse::json::string_to_path(
					FCPPT_TEXT("highlight-threshold"))))),
	blur_filter_(
		_parent_system.filter_manager_,
		_parent_system.texture_manager_,
		fruitlib::pp::filter::texture_size(
			sge::parse::json::find_and_convert_member<sge::renderer::dim2>(
				_parent_system.configuration_,
				sge::parse::json::string_to_path(
					FCPPT_TEXT("bloom-size")))),
		fruitlib::pp::filter::iterations(
			sge::parse::json::find_and_convert_member<fruitlib::pp::filter::iterations::value_type>(
				_parent_system.configuration_,
				sge::parse::json::string_to_path(
					FCPPT_TEXT("bloom-iterations"))))),
	add_filter_(
		_parent_system.filter_manager_,
		_parent_system.texture_manager_,
		fruitlib::pp::filter::texture_size(
			fruitlib::pp::texture::use_screen_size())),
	desaturate_filter_(
		_parent_system.filter_manager_,
		_parent_system.texture_manager_,
		fruitlib::pp::filter::texture_size(
			fruitlib::pp::texture::use_screen_size()),
		fruitlib::pp::filter::desaturate::scaling_factor(
			0.0f))
{
	system_.add_filter(
		rtt_filter_,
		FCPPT_TEXT("source"),
		fruitlib::pp::dependency_set());

	system_.add_filter(
		highlight_filter_,
		FCPPT_TEXT("highlight"),
		fruitlib::pp::dependency_set{
			FCPPT_TEXT("source")
		}
	);

	system_.add_filter(
		blur_filter_,
		FCPPT_TEXT("blur"),
		fruitlib::pp::dependency_set{
			FCPPT_TEXT("highlight")
		}
	);

	system_.add_filter(
		add_filter_,
		FCPPT_TEXT("add"),
		fruitlib::pp::dependency_set{
			FCPPT_TEXT("blur"),
			FCPPT_TEXT("source")
		}
	);

	system_.add_filter(
		desaturate_filter_,
		FCPPT_TEXT("desaturate"),
		fruitlib::pp::dependency_set{
			FCPPT_TEXT("add")
		}
	);
}

void
fruitapp::postprocessing::cg::subsystems::main::render_result(
	sge::renderer::context::core &_context)
{
	if(active_)
		system_.render_result(
			_context);
}

void
fruitapp::postprocessing::cg::subsystems::main::loading_progress(
	sge::renderer::scalar const _progress)
{
	desaturate_filter_.scaling(
		fruitlib::pp::filter::desaturate::scaling_factor(
			_progress));
}

void
fruitapp::postprocessing::cg::subsystems::main::react(
	fruitlib::scenic::events::update const &)
{
	if(active_)
		system_.update();
}

void
fruitapp::postprocessing::cg::subsystems::main::toggle_active()
{
	active_ =
		!active_;
}

fruitlib::pp::texture::counted_instance const
fruitapp::postprocessing::cg::subsystems::main::result_texture()
{
	return
		system_.result_texture();
}

fruitapp::postprocessing::cg::subsystems::main::~main()
{
	parent_system_.main_destroyed();
}
