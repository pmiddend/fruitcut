#include "game_logic.hpp"
#include "fruit/manager.hpp"
#include "../json/find_member.hpp"
#include "../json/parse_color.hpp"
#include "../font/particle/base_parameters.hpp"
#include "../font/color_animation.hpp"
#include "../font/scale_animation.hpp"
#include "../time_format/duration_to_string.hpp"
#include "../time_format/milliseconds.hpp"
#include "../time_format/seconds.hpp"
#include "../font/color.hpp"
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
#include <sge/time/second.hpp>
#include <sge/time/millisecond.hpp>
#include <sge/time/unit.hpp>
#include <fcppt/tr1/functional.hpp>
#include <fcppt/math/box/basic_impl.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/text.hpp>
#include <boost/lexical_cast.hpp>
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
	font::system &_font_system,
	sge::renderer::device &_renderer,
	sge::viewport::manager &_viewport)
:
	score_(
		static_cast<fruitcut::app::score>(
			0)),
	round_timer_(
		sge::time::second(
			json::find_member<sge::time::unit>(
				_config_file,
				FCPPT_TEXT("ingame/round-secs"))),
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
	score_font_(
		fruitcut::font::particle::base_parameters(
			_font_system,
			FCPPT_TEXT("score"),
			SGE_FONT_TEXT_LIT("0"),
			sge::font::rect::null(),
			sge::font::text::align_h::left,
			sge::font::text::align_v::top,
			sge::font::text::flags::none),
		fcppt::assign::make_container<font::color_animation::value_sequence>
			(font::color_animation::value_type(
				sge::time::second(1),
				json::parse_color<font::color>(
					json::find_member<sge::parse::json::value>(
						_config_file,
						FCPPT_TEXT("ingame/score-font-color"))))),
		fcppt::assign::make_container<font::scale_animation::value_sequence>
			(font::scale_animation::value_type(
				sge::time::second(
					1),
				static_cast<sge::renderer::scalar>(
					1))),
		_time_callback),
	timer_font_(
		fruitcut::font::particle::base_parameters(
			_font_system,
			FCPPT_TEXT("timer"),
			SGE_FONT_TEXT_LIT("you shouldn't see this!"),
			sge::font::rect::null(),
			sge::font::text::align_h::center,
			sge::font::text::align_v::center,
			sge::font::text::flags::none),
		fcppt::assign::make_container<font::color_animation::value_sequence>
			(font::color_animation::value_type(
				sge::time::second(1),
				json::parse_color<sge::image::color::rgba8>(
					json::find_member<sge::parse::json::value>(
						_config_file,
						FCPPT_TEXT("ingame/timer-font-color"))))),
		fcppt::assign::make_container<font::scale_animation::value_sequence>
			(font::scale_animation::value_type(
				sge::time::second(
					1),
				static_cast<sge::renderer::scalar>(
					1))),
		_time_callback),
	timer_update_timer_(
		sge::time::millisecond(
			json::find_member<sge::time::unit>(
				_config_file,
				FCPPT_TEXT("ingame/timer-update-msecs"))),
		sge::time::activation_state::active,
		_time_callback),
	renderer_(
		_renderer)
{
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
fruitcut::app::game_logic::update()
{
	if(!timer_update_timer_.update_b())
		return;

	timer_font_.text(
		time_format::duration_to_string<sge::font::text::string>(
			sge::time::duration(
				round_timer_.time_left()),
			time_format::seconds + SGE_FONT_TEXT_LIT(":") + time_format::milliseconds));
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
	
	score_font_.text(
		boost::lexical_cast<sge::font::text::string>(
			score_));
}

void
fruitcut::app::game_logic::viewport_changed()
{
	sge::font::dim const &viewport_dim = 
		fcppt::math::dim::structure_cast<sge::font::dim>(
			renderer_.onscreen_target()->viewport().get().size());

	score_font_.bounding_box(
		sge::font::rect(
			sge::font::pos::null(),
			viewport_dim));

	timer_font_.bounding_box(
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
