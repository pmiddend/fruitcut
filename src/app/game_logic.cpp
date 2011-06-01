#include "fruit/cut_context.hpp"
#include "../fruitlib/font/cache.hpp"
#include "../fruitlib/font/object_parameters.hpp"
#include "../fruitlib/json/find_member.hpp"
#include "../fruitlib/json/parse_color.hpp"
#include "../fruitlib/string_to_duration_exn.hpp"
#include "../fruitlib/time_format/duration_to_string.hpp"
#include "../fruitlib/time_format/milliseconds.hpp"
#include "../fruitlib/time_format/seconds.hpp"
#include "fruit/manager.hpp"
#include "game_logic.hpp"
#include <sge/font/dim.hpp>
#include <sge/font/pos.hpp>
#include <sge/font/rect.hpp>
#include <sge/font/text/align_h.hpp>
#include <sge/font/text/align_v.hpp>
#include <sge/font/text/flags_none.hpp>
#include <sge/font/text/lit.hpp>
#include <sge/font/unit.hpp>
#include <sge/image/color/rgba8.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/onscreen_target.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/renderer/viewport.hpp>
#include <sge/time/activation_state.hpp>
#include <sge/time/duration.hpp>
#include <sge/time/unit.hpp>
#include <sge/viewport/manager.hpp>
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
	fruitlib::font::cache &_font_cache,
	overlay &_overlay,
	sge::renderer::device &_renderer,
	sge::viewport::manager &_viewport)
:
	area_score_factor_(
		fruitlib::json::find_member<fruit::area::value_type>(
			_config_file,
			FCPPT_TEXT("ingame/area-score-factor"))),
	score_(
		0),
	iterating_score_(
		score_),
	round_timer_(
		fruitlib::string_to_duration_exn<sge::time::duration>(
			fruitlib::json::find_member<fcppt::string>(
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
				std::tr1::placeholders::_1))),
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
		fruitlib::font::object_parameters(
			_font_cache.metrics(
				FCPPT_TEXT("score")),
			_font_cache.drawer(
				FCPPT_TEXT("score")),
			SGE_FONT_TEXT_LIT("0"),
			sge::font::rect::null(),
			sge::font::text::align_h::left,
			sge::font::text::align_v::top,
			sge::font::text::flags::none),
		fruitlib::json::parse_color<sge::image::color::rgba8>(
			fruitlib::json::find_member<sge::parse::json::value>(
				_config_file,
				FCPPT_TEXT("ingame/score-font-color"))),
		static_cast<fruitlib::scenic::scale>(
			1)),
	timer_font_node_(
		fruitlib::font::object_parameters(
			_font_cache.metrics(
				FCPPT_TEXT("score")),
			_font_cache.drawer(
				FCPPT_TEXT("score")),
			SGE_FONT_TEXT_LIT("0"),
			sge::font::rect::null(),
			sge::font::text::align_h::center,
			sge::font::text::align_v::top,
			sge::font::text::flags::none),
		fruitlib::json::parse_color<sge::image::color::rgba8>(
			fruitlib::json::find_member<sge::parse::json::value>(
				_config_file,
				FCPPT_TEXT("ingame/timer-font-color"))),
		static_cast<fruitlib::scenic::scale>(
			1)),
	score_increase_timer_(
		fruitlib::string_to_duration_exn<sge::time::duration>(
			fruitlib::json::find_member<fcppt::string>(
				_config_file,
				FCPPT_TEXT("ingame/score-increase-timer"))),
		sge::time::activation_state::active,
		_time_callback),
	renderer_(
		_renderer)
{
	_overlay.insert_dont_care(
		score_font_node_);
	_overlay.insert_dont_care(
		timer_font_node_);
	viewport_changed();
}

bool
fruitcut::app::game_logic::finished() const
{
	return 
		round_timer_.expired();
}

fruitcut::app::score::value_type
fruitcut::app::game_logic::score() const
{
	return 
		score_;
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
	fruit::cut_context const &context)
{
	increase_score(
		app::score(
			static_cast<app::score::value_type>(
				context.area() * area_score_factor_)));
}

void
fruitcut::app::game_logic::viewport_changed()
{
	sge::font::dim const &viewport_dim = 
		fcppt::math::dim::structure_cast<sge::font::dim>(
			renderer_.onscreen_target().viewport().get().size());

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
		fruitlib::time_format::duration_to_string<sge::font::text::string>(
			sge::time::duration(
				round_timer_.time_left()),
			fruitlib::time_format::seconds));

	if(score_increase_timer_.update_b())
	{
		iterating_score_ += 
			(score_ - iterating_score_)/10;
		score_font_node_.object().text(
			fcppt::lexical_cast<sge::font::text::string>(
				iterating_score_));
	}
}

void
fruitcut::app::game_logic::render()
{
}

void
fruitcut::app::game_logic::increase_score(
	fruitcut::app::score const &s)
{
	score_ += 
		s.get();
}
