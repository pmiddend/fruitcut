#include "postprocessing.hpp"
#include "../media_path.hpp"
#include "../fruitlib/json/find_member.hpp"
#include "../fruitlib/pp/dependency_set.hpp"
#include "../fruitlib/pp/texture/depth_stencil_format.hpp"
#include "../fruitlib/pp/texture/use_screen_size.hpp"
#include <sge/font/text/lit.hpp>
#include <sge/font/text/from_fcppt_string.hpp>
#include <sge/font/text/to_fcppt_string.hpp>
#include <sge/console/object.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/dim2.hpp>
#include <sge/renderer/scalar.hpp>
#include <fcppt/text.hpp>
#include <fcppt/string.hpp>
#include <fcppt/function/object.hpp>
#include <fcppt/tr1/functional.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/assign/make_container.hpp>
#include <iostream>

fruitcut::app::postprocessing::postprocessing(
	sge::renderer::device &_renderer,
	sge::console::object &console,
	fcppt::function::object<void ()> const &render_callback,
	sge::parse::json::object const &config)
:
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
		fruitlib::json::find_member<sge::renderer::dim2>(
			config,
			FCPPT_TEXT("bloom-size")),
		fruitlib::json::find_member<sge::renderer::scalar>(
			config,
			FCPPT_TEXT("highlight-threshold"))),
	blur_filter_(
		_renderer,
		filter_manager_,
		texture_manager_,
		fruitlib::json::find_member<sge::renderer::dim2>(
			config,
			FCPPT_TEXT("bloom-size")),
		fruitlib::json::find_member<fruitlib::pp::filter::blur::size_type>(
			config,
			FCPPT_TEXT("bloom-iterations"))),
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
	list_connection_(
		console.insert(
			SGE_FONT_TEXT_LIT("pplist"),
			std::tr1::bind(
				&postprocessing::list_filters,
				this,
				std::tr1::placeholders::_1,
				std::tr1::placeholders::_2),
			SGE_FONT_TEXT_LIT("List all filters"))),
	toggle_connection_(
		console.insert(
			SGE_FONT_TEXT_LIT("pptoggle"),
				std::tr1::bind(
					&postprocessing::toggle_filter,
					this,
					std::tr1::placeholders::_1,
					std::tr1::placeholders::_2),
				SGE_FONT_TEXT_LIT("Toggle a specific filter"))),
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
fruitcut::app::postprocessing::update()
{
	if (active_)
		system_.update();
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
fruitcut::app::postprocessing::list_filters(
	sge::console::arg_list const &args,
	sge::console::object &obj)
{
	if(args.size() != 1)
		obj.emit_error(
			SGE_FONT_TEXT_LIT("warning: ")+args[0]+SGE_FONT_TEXT_LIT(" doesn't take any parameters"));
		
	for(
		fruitlib::pp::system::filter_name_set::const_iterator s = 
			system_.filter_names().begin();
		s != system_.filter_names().end();
		++s)
		obj.emit_message(
			sge::font::text::from_fcppt_string(
				*s));
}

void
fruitcut::app::postprocessing::toggle_filter(
	sge::console::arg_list const &args,
	sge::console::object &obj)
{
	if (args.size() != 2)
	{
		obj.emit_error(
			SGE_FONT_TEXT_LIT("error: expected one argument, the name of the filter"));
		return;
	}

	system_.toggle_filter(
		sge::font::text::to_fcppt_string(
			args[1]));
}
