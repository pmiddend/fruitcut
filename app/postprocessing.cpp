#include "postprocessing.hpp"
#include "../json/find_member.hpp"
#include "../pp/dependency_set.hpp"
#include "../pp/texture/depth_stencil_format.hpp"
#include <sge/font/text/lit.hpp>
#include <sge/font/text/from_fcppt_string.hpp>
#include <sge/font/text/to_fcppt_string.hpp>
#include <sge/console/object.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/dim2.hpp>
#include <sge/renderer/scalar.hpp>
#include <fcppt/text.hpp>
#include <fcppt/string.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/assign/make_container.hpp>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <iostream>

fruitcut::app::postprocessing::postprocessing(
	sge::renderer::device_ptr const &_renderer,
	sge::console::object &console,
	boost::function<void ()> const &render_callback,
	sge::parse::json::object const &config)
:
	system_(
		_renderer),
	texture_manager_(
		_renderer),
	rtt_filter_(
		_renderer,
		texture_manager_,
		fcppt::math::dim::structure_cast<sge::renderer::dim2>(
			_renderer->screen_size()),
		render_callback,
		pp::texture::depth_stencil_format::d32),
	ssaa_filter_(
		_renderer,
		texture_manager_,
		fcppt::math::dim::structure_cast<sge::renderer::dim2>(
			_renderer->screen_size())),
	highlight_filter_(
		_renderer,
		texture_manager_,
		json::find_member<sge::renderer::dim2>(
			config,
			FCPPT_TEXT("bloom-size")),
		json::find_member<sge::renderer::scalar>(
			config,
			FCPPT_TEXT("highlight-threshold"))),
	blur_filter_(
		_renderer,
		texture_manager_,
		json::find_member<sge::renderer::dim2>(
			config,
			FCPPT_TEXT("bloom-size")),
		json::find_member<pp::filter::blur::size_type>(
			config,
			FCPPT_TEXT("bloom-iterations"))),
	add_filter_(
		_renderer,
		texture_manager_,
		fcppt::math::dim::structure_cast<sge::renderer::dim2>(
			_renderer->screen_size())),
	desaturate_filter_(
		_renderer,
		texture_manager_,
		fcppt::math::dim::structure_cast<sge::renderer::dim2>(
			_renderer->screen_size()),
		static_cast<sge::renderer::scalar>(
			0.0)),
	list_connection_(
		console.insert(
			SGE_FONT_TEXT_LIT("pplist"),
			boost::bind(
				&postprocessing::list_filters,
				this,
				_1,
				_2),
			SGE_FONT_TEXT_LIT("List all filters"))),
	toggle_connection_(
		console.insert(
			SGE_FONT_TEXT_LIT("pptoggle"),
				boost::bind(
					&postprocessing::toggle_filter,
					this,
					_1,
					_2),
				SGE_FONT_TEXT_LIT("Toggle a specific filter")))
{
	system_.add_filter(
		rtt_filter_,
		FCPPT_TEXT("source"),
		fruitcut::pp::dependency_set());

	system_.add_filter(
		ssaa_filter_,
		FCPPT_TEXT("ssaa"),
		fcppt::assign::make_container<fruitcut::pp::dependency_set>
			(FCPPT_TEXT("source")));

	system_.add_filter(
		highlight_filter_,
		FCPPT_TEXT("highlight"),
		fcppt::assign::make_container<fruitcut::pp::dependency_set>
			(FCPPT_TEXT("ssaa")));

	system_.add_filter(
		blur_filter_,
		FCPPT_TEXT("blur"),
		fcppt::assign::make_container<fruitcut::pp::dependency_set>
			(FCPPT_TEXT("highlight")));

	system_.add_filter(
		add_filter_,
		FCPPT_TEXT("add"),
		fcppt::assign::make_container<fruitcut::pp::dependency_set>
			(FCPPT_TEXT("blur"))
			(FCPPT_TEXT("ssaa")));

	system_.add_filter(
		desaturate_filter_,
		FCPPT_TEXT("desaturate"),
		fcppt::assign::make_container<fruitcut::pp::dependency_set>
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

sge::renderer::texture_ptr const
fruitcut::app::postprocessing::result_texture()
{
	return 
		system_.result_texture();
}

fruitcut::pp::filter::desaturate &
fruitcut::app::postprocessing::desaturate_filter()
{
	return desaturate_filter_;
}

fruitcut::pp::texture::manager &
fruitcut::app::postprocessing::texture_manager()
{
	return texture_manager_;
}

void
fruitcut::app::postprocessing::active(
	bool const _active)
{
	active_ = 
		_active;
}

void
fruitcut::app::postprocessing::list_filters(
	sge::console::arg_list const &args,
	sge::console::object &obj)
{
	if(args.size() != 1)
		obj.emit_error(
			SGE_FONT_TEXT_LIT("warning: ")+args[0]+SGE_FONT_TEXT_LIT(" doesn't take any parameters"));
		
	BOOST_FOREACH(
		fcppt::string const &s,
		system_.filter_names())
		obj.emit_message(
			sge::font::text::from_fcppt_string(
				s));
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
