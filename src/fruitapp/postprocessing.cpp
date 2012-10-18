#include <fruitapp/postprocessing.hpp>
#include <fruitapp/viewport/manager.hpp>
#include <fruitlib/media_path.hpp>
#include <fruitlib/pp/dependency_set.hpp>
#include <fruitlib/pp/texture/depth_stencil_format.hpp>
#include <fruitlib/pp/texture/use_screen_size.hpp>
#include <sge/image/colors.hpp>
#include <sge/parse/json/find_and_convert_member.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <sge/renderer/dim2.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/renderer/device/core.hpp>
#include <sge/shader/context.hpp>
#include <fcppt/text.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/function/object.hpp>
#include <fcppt/signal/connection.hpp>
#include <fcppt/config/external_begin.hpp>
#include <iostream>
#include <fcppt/config/external_end.hpp>


fruitapp::postprocessing::postprocessing(
	fruitlib::scenic::optional_parent const &_parent,
	sge::shader::context &_shader_context,
	render_callback const &render_callback,
	sge::parse::json::object const &config,
	fruitapp::viewport::manager &_viewport_manager)
:
	node_base(
		_parent),
	texture_manager_(
		_shader_context.renderer()),
	filter_manager_(
		_shader_context,
		fruitlib::pp::filter::base_path(
			fruitlib::media_path() / FCPPT_TEXT("shaders") / FCPPT_TEXT("postprocessing"))),
	system_(
		filter_manager_),
	rtt_filter_(
		filter_manager_,
		texture_manager_,
		fruitlib::pp::filter::texture_size(
			fruitlib::pp::texture::use_screen_size()),
		sge::renderer::clear::parameters()
			.back_buffer(
				sge::renderer::clear::back_buffer_value(
					sge::image::colors::black()))
			.depth_buffer(
				sge::renderer::clear::depth_buffer_value(
					1.0f)),
		render_callback,
		fruitlib::pp::texture::depth_stencil_format::d32),
/*
	ssaa_filter_(
		filter_manager_,
		texture_manager_,
		fruitlib::pp::filter::texture_size(
			fruitlib::pp::texture::use_screen_size())),
*/
	highlight_filter_(
		filter_manager_,
		texture_manager_,
		fruitlib::pp::filter::texture_size(
			sge::parse::json::find_and_convert_member<sge::renderer::dim2>(
				config,
				sge::parse::json::path(
					FCPPT_TEXT("bloom-size")))),
		fruitlib::pp::filter::highlight::threshold_factor(
			sge::parse::json::find_and_convert_member<sge::renderer::scalar>(
				config,
				sge::parse::json::path(
					FCPPT_TEXT("highlight-threshold"))))),
	blur_filter_(
		filter_manager_,
		texture_manager_,
		fruitlib::pp::filter::texture_size(
			sge::parse::json::find_and_convert_member<sge::renderer::dim2>(
				config,
				sge::parse::json::path(
					FCPPT_TEXT("bloom-size")))),
		fruitlib::pp::filter::iterations(
			sge::parse::json::find_and_convert_member<fruitlib::pp::filter::iterations::value_type>(
				config,
				sge::parse::json::path(
					FCPPT_TEXT("bloom-iterations"))))),
	add_filter_(
		filter_manager_,
		texture_manager_,
		fruitlib::pp::filter::texture_size(
			fruitlib::pp::texture::use_screen_size())),
	desaturate_filter_(
		filter_manager_,
		texture_manager_,
		fruitlib::pp::filter::texture_size(
			fruitlib::pp::texture::use_screen_size()),
		fruitlib::pp::filter::desaturate::scaling_factor(
			0.0f)),
	active_(
		true),
	viewport_change_connection_(
		_viewport_manager.change_callback(
			std::tr1::bind(
				&postprocessing::viewport_change,
				this,
				std::tr1::placeholders::_1),
			fruitapp::viewport::trigger_early(
				true)))
{
	system_.add_filter(
		rtt_filter_,
		FCPPT_TEXT("source"),
		fruitlib::pp::dependency_set());

	/*
	system_.add_filter(
		ssaa_filter_,
		FCPPT_TEXT("ssaa"),
		fcppt::assign::make_container<fruitlib::pp::dependency_set>
			(FCPPT_TEXT("source")));
	*/

	system_.add_filter(
		highlight_filter_,
		FCPPT_TEXT("highlight"),
		fcppt::assign::make_container<fruitlib::pp::dependency_set>
//			(FCPPT_TEXT("ssaa")));
			(FCPPT_TEXT("source")));

	system_.add_filter(
		blur_filter_,
		FCPPT_TEXT("blur"),
		fcppt::assign::make_container<fruitlib::pp::dependency_set>
			(FCPPT_TEXT("highlight")));

	system_.add_filter(
		add_filter_,
		FCPPT_TEXT("add"),
		fcppt::assign::make_container<fruitlib::pp::dependency_set>
			(FCPPT_TEXT("blur"))
//			(FCPPT_TEXT("ssaa")));
			(FCPPT_TEXT("source")));

	system_.add_filter(
		desaturate_filter_,
		FCPPT_TEXT("desaturate"),
		fcppt::assign::make_container<fruitlib::pp::dependency_set>
			(FCPPT_TEXT("add")));
}

void
fruitapp::postprocessing::render_result(
	sge::renderer::context::core &_context)
{
	if (active_)
		system_.render_result(
			_context);
}

sge::renderer::texture::planar_shared_ptr
fruitapp::postprocessing::result_texture()
{
	return
		system_.result_texture();
}

fruitlib::pp::filter::desaturate &
fruitapp::postprocessing::desaturate_filter()
{
	return desaturate_filter_;
}

fruitlib::pp::texture::manager &
fruitapp::postprocessing::texture_manager()
{
	return texture_manager_;
}

fruitlib::pp::filter::manager &
fruitapp::postprocessing::filter_manager()
{
	return filter_manager_;
}

bool
fruitapp::postprocessing::active()
{
	return
		active_;
}

void
fruitapp::postprocessing::active(
	bool const _active)
{
	active_ =
		_active;
}

void
fruitapp::postprocessing::react(
	fruitlib::scenic::events::update const &)
{
	if (active_)
		system_.update();
}

fruitapp::postprocessing::~postprocessing()
{
}

void
fruitapp::postprocessing::viewport_change(
	sge::renderer::target::viewport const &)
{
	texture_manager_.clear_screen_textures();
}
