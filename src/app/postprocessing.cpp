#include "postprocessing.hpp"
#include "../media_path.hpp"
#include "../fruitlib/json/find_and_convert_member.hpp"
#include "../fruitlib/pp/dependency_set.hpp"
#include "../fruitlib/pp/texture/depth_stencil_format.hpp"
#include "../fruitlib/scenic/events/update.hpp"
#include "../fruitlib/pp/texture/use_screen_size.hpp"
#include <sge/renderer/device.hpp>
#include <sge/renderer/dim2.hpp>
#include <sge/renderer/scalar.hpp>
#include <fcppt/text.hpp>
#include <fcppt/function/object.hpp>
#include <fcppt/assign/make_container.hpp>
#include <iostream>

fruitcut::app::postprocessing::postprocessing(
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
fruitcut::app::postprocessing::render_result()
{
	if (active_)
		system_.render_result();
}

sge::renderer::texture::planar_ptr const
fruitcut::app::postprocessing::result_texture()
{
	return 
		system_.result_texture();
}

fruitcut::fruitlib::pp::filter::desaturate &
fruitcut::app::postprocessing::desaturate_filter()
{
	return desaturate_filter_;
}

fruitcut::fruitlib::pp::texture::manager &
fruitcut::app::postprocessing::texture_manager()
{
	return texture_manager_;
}

fruitcut::fruitlib::pp::filter::manager &
fruitcut::app::postprocessing::filter_manager()
{
	return filter_manager_;
}

bool
fruitcut::app::postprocessing::active()
{
	return 
		active_;
}

void
fruitcut::app::postprocessing::active(
	bool const _active)
{
	active_ = 
		_active;
}

void
fruitcut::app::postprocessing::viewport_changed()
{
	texture_manager_.clear_screen_textures();
}

void
fruitcut::app::postprocessing::react(
	fruitlib::scenic::events::update const &)
{
	if (active_)
		system_.update();
}
