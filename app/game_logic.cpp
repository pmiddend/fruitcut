#include "game_logic.hpp"
#include "../string_to_duration_exn.hpp"
#include "fruit/manager.hpp"
#include "../json/find_member.hpp"
#include "../json/parse_color.hpp"
#include "../time_format/duration_to_string.hpp"
#include "../time_format/milliseconds.hpp"
#include "../time_format/seconds.hpp"
#include "../font/cache.hpp"
#include "../font/object_parameters.hpp"
#include <sge/font/pos.hpp>
#include <sge/font/unit.hpp>
#include <sge/font/rect.hpp>
#include <sge/font/dim.hpp>
#include <sge/font/text/align_h.hpp>
#include <sge/font/text/align_v.hpp>
#include <sge/font/text/flags_none.hpp>
#include <sge/font/text/lit.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/onscreen_target.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/renderer/viewport.hpp>
#include <sge/viewport/manager.hpp>
#include <sge/time/activation_state.hpp>
#include <sge/time/duration.hpp>
#include <sge/time/unit.hpp>
#include <sge/image/color/rgba8.hpp>
#include <fcppt/tr1/functional.hpp>
#include <fcppt/math/box/basic_impl.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/text.hpp>
#include <fcppt/string.hpp>
#include <fcppt/lexical_cast.hpp>
#include <iostream>

fruitcut::app::game_logic::game_logic(
	sge::time::callback const &_time_callback,
	// to get round seconds and stuff
	sge::parse::json::object const &_config_file,
	// to get 
	// - "fruit was cut"
	// - "fruit was deleted" 
	// - "fruit was added" (we could consult the spawner for that, but that's not The Right Thing)
	fruit::manager &_fruit_manager,
	font::cache &_font_cache,
	overlay &_overlay,
	sge::renderer::device &_renderer,
	sge::viewport::manager &_viewport)
:
	score_(
		static_cast<fruitcut::app::score>(
			0)),
	round_timer_(
		string_to_duration_exn<sge::time::duration>(
			json::find_member<fcppt::string>(
				_config_file,
				FCPPT_TEXT("ingame/round-time"))),
		sge::time::activation_state::active,
		_time_callback),
	fruit_added_connection_(
		_fruit_manager.spawn_callback(
			std::tr1::bind(
				&game_logic::fruit_added,
				this,
				std::tr1::placeholders::_1))),
	fruit_cut_connection_(
		_fruit_manager.cut_callback(
			std::tr1::bind(
				&game_logic::fruit_cut,
				this,
				std::tr1::placeholders::_1,
				std::tr1::placeholders::_2,
				std::tr1::placeholders::_3,
				std::tr1::placeholders::_4))),
	fruit_removed_connection_(
		_fruit_manager.remove_callback(
			std::tr1::bind(
				&game_logic::fruit_removed,
				this,
				std::tr1::placeholders::_1))),
	viewport_changed_connection_(
		_viewport.manage_callback(
			std::tr1::bind(
				&game_logic::viewport_changed,
				this))),
	score_font_node_(
		fruitcut::font::object_parameters(
			_font_cache.metrics(
				FCPPT_TEXT("score")),
			_font_cache.drawer(
				FCPPT_TEXT("score")),
			SGE_FONT_TEXT_LIT("0"),
			sge::font::rect::null(),
			sge::font::text::align_h::left,
			sge::font::text::align_v::top,
			sge::font::text::flags::none),
		json::parse_color<sge::image::color::rgba8>(
			json::find_member<sge::parse::json::value>(
				_config_file,
				FCPPT_TEXT("ingame/score-font-color"))),
		static_cast<scenic::scale>(
			1)),
	timer_font_node_(
		fruitcut::font::object_parameters(
			_font_cache.metrics(
				FCPPT_TEXT("score")),
			_font_cache.drawer(
				FCPPT_TEXT("score")),
			SGE_FONT_TEXT_LIT("0"),
			sge::font::rect::null(),
			sge::font::text::align_h::center,
			sge::font::text::align_v::top,
			sge::font::text::flags::none),
		json::parse_color<sge::image::color::rgba8>(
			json::find_member<sge::parse::json::value>(
				_config_file,
				FCPPT_TEXT("ingame/timer-font-color"))),
		static_cast<scenic::scale>(
			1)),
	renderer_(
		_renderer)
{
	_overlay.children().push_back(
		score_font_node_);
	_overlay.children().push_back(
		timer_font_node_);
	viewport_changed();
}

bool
fruitcut::app::game_logic::finished() const
{
	return 
		round_timer_.expired();
}

fruitcut::app::score
fruitcut::app::game_logic::score() const
{
	return score_;
}

void
fruitcut::app::game_logic::fruit_added(
	fruit::object const &)
{
}

void
fruitcut::app::game_logic::fruit_removed(
	fruit::object const &)
{
}

void
fruitcut::app::game_logic::fruit_cut(
	fruit::object const &,
	fruit::object const &,
	fruit::object const &,
	sge::renderer::scalar const area)
{
	score_ = 
		static_cast<fruitcut::app::score>(
			score_ + 
			static_cast<fruitcut::app::score>(
				area * static_cast<sge::renderer::scalar>(100))); 
	
	score_font_node_.object().text(
		fcppt::lexical_cast<sge::font::text::string>(
			score_));
}

void
fruitcut::app::game_logic::viewport_changed()
{
	sge::font::dim const &viewport_dim = 
		fcppt::math::dim::structure_cast<sge::font::dim>(
			renderer_.onscreen_target()->viewport().get().size());

	score_font_node_.object().bounding_box(
		sge::font::rect(
			sge::font::pos::null(),
			viewport_dim));

	timer_font_node_.object().bounding_box(
		sge::font::rect(
			sge::font::pos::null(),
			sge::font::dim(
				viewport_dim.w(),
				static_cast<sge::font::unit>(
					static_cast<sge::renderer::scalar>(
						viewport_dim.h()) * 
					static_cast<sge::renderer::scalar>(
						0.2)))));
}

void
fruitcut::app::game_logic::update()
{
	timer_font_node_.object().text(
		time_format::duration_to_string<sge::font::text::string>(
			sge::time::duration(
				round_timer_.time_left()),
			time_format::seconds));
}

void
fruitcut::app::game_logic::render()
{
}
