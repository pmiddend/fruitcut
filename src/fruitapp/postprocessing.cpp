#include "postprocessing.hpp"
#include <media_path.hpp>
#include <fruitlib/json/find_and_convert_member.hpp>
#include <fruitlib/pp/dependency_set.hpp>
#include <fruitlib/pp/texture/depth_stencil_format.hpp>
#include <fruitlib/scenic/events/update.hpp>
#include <fruitlib/scenic/events/viewport_change.hpp>
#include <fruitlib/pp/texture/use_screen_size.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/dim2.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/renderer/state/state.hpp>
#include <sge/image/colors.hpp>
#include <fcppt/text.hpp>
#include <fcppt/function/object.hpp>
#include <fcppt/assign/make_container.hpp>
#include <iostream>

fruitapp::postprocessing::postprocessing(
	fruitlib::scenic::optional_parent const &_parent,
	sge::renderer::device &_renderer,
	fcppt::function::object<void ()> const &render_callback,
	sge::parse::json::object const &config)
:
	node_base(
		_parent),
	texture_manager_(
		_renderer),
	filter_manager_(
		_renderer,
		fruitcut::media_path()/FCPPT_TEXT("shaders")),
	system_(
		filter_manager_),
	rtt_filter_(
		_renderer,
		sge::renderer::state::list
			(sge::renderer::state::depth_func::less)
			(sge::renderer::state::bool_::clear_depth_buffer = true)
			(sge::renderer::state::bool_::clear_back_buffer = true)
			(sge::renderer::state::color::back_buffer_clear_color = sge::image::colors::black())
			(sge::renderer::state::float_::depth_buffer_clear_val = 1.0f),
		texture_manager_,
		fruitlib::pp::texture::use_screen_size(),
		render_callback,
		fruitlib::pp::texture::depth_stencil_format::d32),
	ssaa_filter_(
		_renderer,
		filter_manager_,
		texture_manager_,
		fruitlib::pp::texture::use_screen_size()),
	highlight_filter_(
		_renderer,
		filter_manager_,
		texture_manager_,
		fruitlib::json::find_and_convert_member<sge::renderer::dim2>(
			config,
			fruitlib::json::path(
				FCPPT_TEXT("bloom-size"))),
		fruitlib::json::find_and_convert_member<sge::renderer::scalar>(
			config,
			fruitlib::json::path(
				FCPPT_TEXT("highlight-threshold")))),
	blur_filter_(
		_renderer,
		filter_manager_,
		texture_manager_,
		fruitlib::json::find_and_convert_member<sge::renderer::dim2>(
			config,
			fruitlib::json::path(
			FCPPT_TEXT("bloom-size"))),
		fruitlib::json::find_and_convert_member<fruitlib::pp::filter::blur::size_type>(
			config,
			fruitlib::json::path(
				FCPPT_TEXT("bloom-iterations")))),
	add_filter_(
		_renderer,
		filter_manager_,
		texture_manager_,
		fruitlib::pp::texture::use_screen_size()),
	desaturate_filter_(
		_renderer,
		filter_manager_,
		texture_manager_,
		fruitlib::pp::texture::use_screen_size(),
		static_cast<sge::renderer::scalar>(
			0.0)),
	active_(
		true)
{
	system_.add_filter(
		rtt_filter_,
		FCPPT_TEXT("source"),
		fruitlib::pp::dependency_set());

	system_.add_filter(
		ssaa_filter_,
		FCPPT_TEXT("ssaa"),
		fcppt::assign::make_container<fruitlib::pp::dependency_set>
			(FCPPT_TEXT("source")));

	system_.add_filter(
		highlight_filter_,
		FCPPT_TEXT("highlight"),
		fcppt::assign::make_container<fruitlib::pp::dependency_set>
			(FCPPT_TEXT("ssaa")));

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
			(FCPPT_TEXT("ssaa")));

	system_.add_filter(
		desaturate_filter_,
		FCPPT_TEXT("desaturate"),
		fcppt::assign::make_container<fruitlib::pp::dependency_set>
			(FCPPT_TEXT("add")));
}

void
fruitapp::postprocessing::render_result()
{
	if (active_)
		system_.render_result();
}

sge::renderer::texture::planar_ptr const
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

void
fruitapp::postprocessing::react(
	fruitlib::scenic::events::viewport_change const &)
{
	texture_manager_.clear_screen_textures();
}
