#include "../fruit/cut_context.hpp"
#include "../../fruitlib/font/cache.hpp"
#include "../../fruitlib/font/object_parameters.hpp"
#include "../../fruitlib/json/find_and_convert_member.hpp"
#include "../../fruitlib/json/parse_color.hpp"
#include "../../fruitlib/time_format/string_to_duration_exn.hpp"
#include "../../fruitlib/time_format/duration_to_string.hpp"
#include "../../fruitlib/time_format/milliseconds.hpp"
#include "../../fruitlib/time_format/seconds.hpp"
#include "../fruit/manager.hpp"
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
#include <sge/image/color/init.hpp>
#include <sge/image/colors.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/onscreen_target.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/renderer/viewport.hpp>
#include <sge/time/activation_state.hpp>
#include <sge/time/duration.hpp>
#include <sge/time/unit.hpp>
#include <sge/time/millisecond.hpp>
#include <sge/viewport/manager.hpp>
#include <mizuiro/color/convert.hpp>
#include <mizuiro/color/homogenous_static.hpp>
#include <mizuiro/color/init.hpp>
#include <mizuiro/color/layout/hsva.hpp>
#include <mizuiro/color/object.hpp>
#include <fcppt/tr1/functional.hpp>
#include <fcppt/math/clamp.hpp>
#include <fcppt/math/box/basic_impl.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/text.hpp>
#include <fcppt/string.hpp>
#include <fcppt/lexical_cast.hpp>
#include <fcppt/format.hpp>
#include <boost/cstdint.hpp>
#include <iostream>

fruitcut::app::game_logic::game_logic::game_logic(
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
		fruitlib::json::find_and_convert_member<fruit::area::value_type>(
			_config_file,
			fruitlib::json::path(
				FCPPT_TEXT("ingame"))
				/ FCPPT_TEXT("area-score-factor"))),
	score_(
		0),
	iterating_score_(
		score_),
	round_timer_(
		fruitlib::time_format::string_to_duration_exn<sge::time::duration>(
			fruitlib::json::find_and_convert_member<fcppt::string>(
				_config_file,
				fruitlib::json::path(
					FCPPT_TEXT("ingame"))
					/ FCPPT_TEXT("round-time"))),
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
			fruitlib::json::find_and_convert_member<sge::parse::json::value>(
				_config_file,
				fruitlib::json::path(
					FCPPT_TEXT("ingame"))
						/FCPPT_TEXT("score-font-color"))),
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
			fruitlib::json::find_and_convert_member<sge::parse::json::value>(
				_config_file,
				fruitlib::json::path(
					FCPPT_TEXT("ingame"))
					/ FCPPT_TEXT("timer-font-color"))),
		static_cast<fruitlib::scenic::scale>(
			1)),
	multiplier_font_node_(
		fruitlib::font::object_parameters(
			_font_cache.metrics(
				FCPPT_TEXT("score")),
			_font_cache.drawer(
				FCPPT_TEXT("score")),
			SGE_FONT_TEXT_LIT(""),
			sge::font::rect::null(),
			sge::font::text::align_h::center,
			sge::font::text::align_v::bottom,
			sge::font::text::flags::none),
			sge::image::colors::white(),
		static_cast<fruitlib::scenic::scale>(
			1)),
	score_increase_timer_(
		fruitlib::time_format::string_to_duration_exn<sge::time::duration>(
			fruitlib::json::find_and_convert_member<fcppt::string>(
				_config_file,
				fruitlib::json::path(
					FCPPT_TEXT("ingame"))
					/ FCPPT_TEXT("score-increase-timer"))),
		sge::time::activation_state::active,
		_time_callback),
	multiplier_timer_(
		fruitlib::time_format::string_to_duration_exn<sge::time::duration>(
			fruitlib::json::find_and_convert_member<fcppt::string>(
				_config_file,
				fruitlib::json::path(
					FCPPT_TEXT("ingame"))
					/ FCPPT_TEXT("multiplier-timer"))),
		sge::time::activation_state::active,
		_time_callback),
	penalty_timer_(
		fruitlib::time_format::string_to_duration_exn<sge::time::duration>(
			fruitlib::json::find_and_convert_member<fcppt::string>(
				_config_file,
				fruitlib::json::path(
					FCPPT_TEXT("ingame"))
					/ FCPPT_TEXT("penalty-timer"))),
		sge::time::activation_state::inactive,
		_time_callback),
	multiplier_(1),
	multi_count_(0),
	renderer_(
		_renderer)
{
	_overlay.insert_dont_care(
		score_font_node_);
	_overlay.insert_dont_care(
		timer_font_node_);
	_overlay.insert_dont_care(
		multiplier_font_node_);
	viewport_changed();
}

bool
fruitcut::app::game_logic::game_logic::finished() const
{
	return 
		round_timer_.expired();
}

fruitcut::app::score::value_type
fruitcut::app::game_logic::game_logic::score() const
{
	return 
		score_;
}

void
fruitcut::app::game_logic::game_logic::fruit_added(
	fruit::object const &)
{
}

void
fruitcut::app::game_logic::game_logic::fruit_removed(
	fruit::object const &)
{
}

void
fruitcut::app::game_logic::game_logic::fruit_cut(
	fruit::cut_context const &context)
{
	fruit::tag_sequence ts = context.old().prototype().tags();
	if (!ts.empty() && ts.front() == "meat")
	{
		penalty_timer_.activate();
		penalty_timer_.reset();
		multiplier_timer_.deactivate();
		multiplier_timer_.reset();
		multiplier_ = 0;
		multi_count_ = 0;
		multiplier_font_node_.scale(
			static_cast<fruitlib::scenic::scale>(2));
		multiplier_font_node_.color(
			sge::image::colors::gray());
	}
	else
		increase_score(
			app::score(
				static_cast<app::score::value_type>(
					static_cast<app::score::value_type>(
						multiplier_) *
					context.area() * 
					area_score_factor_)));
	if (!multiplier_timer_.expired())
	{
		++multi_count_;
		if (multi_count_ > 5)
		{
			++multiplier_;
			multi_count_ = 0;
			multiplier_font_node_.scale(
				static_cast<fruitlib::scenic::scale>(
					0.75f *
					std::sqrt(static_cast<float>(multiplier_ + 1))));
		}
		multiplier_timer_.reset();
		if (multiplier_ != 1)
			multiplier_font_node_.object().text(
					fcppt::lexical_cast<sge::font::text::string>(
						multiplier_) +
					SGE_FONT_TEXT_LIT("x"));
	}
}

void
fruitcut::app::game_logic::game_logic::viewport_changed()
{
	sge::font::dim const &viewport_dim = 
		fcppt::math::dim::structure_cast<sge::font::dim>(
			renderer_.onscreen_target().viewport().get().size());

	score_font_node_.object().bounding_box(
		sge::font::rect(
			sge::font::pos::null(),
			viewport_dim));

	multiplier_font_node_.object().bounding_box(
		sge::font::rect(
			sge::font::pos::null(),
			sge::font::dim(
				viewport_dim.w(),
				static_cast<sge::font::unit>(
					static_cast<sge::renderer::scalar>(
						viewport_dim.h()) * 
					static_cast<sge::renderer::scalar>(
						0.9)))));

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
fruitcut::app::game_logic::game_logic::update()
{
	if (penalty_timer_.active() && penalty_timer_.expired())
	{
		penalty_timer_.reset();
		penalty_timer_.deactivate();
		multiplier_timer_.reset();
		multiplier_timer_.activate();
		multiplier_ = 1;
		multiplier_font_node_.object().text(
				SGE_FONT_TEXT_LIT(""));
	}
	if (multiplier_timer_.active() && multiplier_timer_.expired())
	{
		multiplier_ = 1;
		multi_count_ = 0;
		multiplier_timer_.reset();
		multiplier_font_node_.object().text(
				SGE_FONT_TEXT_LIT(""));
	}
	else if (multiplier_timer_.active())
	{
		multiplier_font_node_.color(
			mizuiro::color::convert<
				sge::image::color::rgba8_format
			>(
			mizuiro::color::object<
				mizuiro::color::homogenous_static<
					boost::uint8_t,
					mizuiro::color::layout::hsva
				>
			>(
			(mizuiro::color::init::hue %= 
					0.34 *
					(1.f - 
						multiplier_timer_.elapsed_frames()))
			(mizuiro::color::init::saturation %= 1.0)
			(mizuiro::color::init::value %= 1.0)
			(mizuiro::color::init::alpha %= 1.0))));
	}
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
fruitcut::app::game_logic::game_logic::render()
{
}

void
fruitcut::app::game_logic::game_logic::increase_score(
	fruitcut::app::score const &s)
{
	score_ += 
		s.get();
}
