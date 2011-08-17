#include <fruitapp/fruit/cut_context.hpp>
#include <fruitlib/font/cache.hpp>
#include <fruitlib/font/object_parameters.hpp>
#include <sge/parse/json/find_and_convert_member.hpp>
#include <fruitlib/json/parse_rgba8_color.hpp>
#include <fruitlib/resource_tree/path.hpp>
#include <fruitlib/time_format/find_and_convert_duration.hpp>
#include <fruitlib/time_format/duration_to_string.hpp>
#include <fruitlib/time_format/milliseconds.hpp>
#include <fruitlib/time_format/seconds.hpp>
#include <fruitapp/fruit/manager.hpp>
#include <fruitapp/depths/overlay.hpp>
#include <fruitapp/game_logic/object.hpp>
#include <sge/font/dim.hpp>
#include <sge/font/pos.hpp>
#include <sge/font/rect.hpp>
#include <sge/font/text/align_h.hpp>
#include <sge/font/text/align_v.hpp>
#include <sge/font/text/flags_none.hpp>
#include <sge/font/text/lit.hpp>
#include <sge/font/unit.hpp>
#include <sge/image/color/rgba8.hpp>
#include <sge/image/color/any/convert.hpp>
#include <sge/image/color/init.hpp>
#include <sge/image/color/convert.hpp>
#include <sge/image/colors.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/onscreen_target.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/renderer/viewport.hpp>
#include <sge/timer/parameters.hpp>
#include <sge/timer/reset_when_expired.hpp>
#include <sge/timer/elapsed_fractional.hpp>
#include <sge/timer/remaining.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <sge/parse/json/value.hpp>
#include <sge/font/text/string.hpp>
#include <sge/font/text/flags.hpp>
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

fruitapp::game_logic::object::object(
	fruitlib::scenic::optional_parent const &_parent,
	fruitapp::ingame_clock const &_clock,
	// to get round seconds and stuff
	sge::parse::json::object const &_config_file,
	// to get
	// - "fruit was cut"
	// - "fruit was deleted"
	// - "fruit was added" (we could consult the spawner for that, but that's not The Right Thing)
	fruitlib::audio::sound_controller &_sound_controller,
	fruit::manager &_fruit_manager,
	fruitlib::font::cache &_font_cache,
	overlay &_overlay,
	sge::renderer::device &_renderer)
:
	node_base(
		_parent),
	area_score_factor_(
		sge::parse::json::find_and_convert_member<fruit::area::value_type>(
			_config_file,
			sge::parse::json::path(
				FCPPT_TEXT("ingame"))
				/ FCPPT_TEXT("area-score-factor"))),
	score_(
		0),
	iterating_score_(
		score_),
	round_timer_(
		fruitapp::ingame_timer::parameters(
			_clock,
			fruitlib::time_format::find_and_convert_duration<fruitapp::ingame_clock::duration>(
				_config_file,
				sge::parse::json::path(
					FCPPT_TEXT("ingame"))
					/ FCPPT_TEXT("round-time")))),
	sound_controller_(
		_sound_controller),
	fruit_added_connection_(
		_fruit_manager.spawn_callback(
			std::tr1::bind(
				&object::fruit_added,
				this,
				std::tr1::placeholders::_1))),
	fruit_cut_connection_(
		_fruit_manager.cut_callback(
			std::tr1::bind(
				&object::fruit_cut,
				this,
				std::tr1::placeholders::_1))),
	fruit_removed_connection_(
		_fruit_manager.remove_callback(
			std::tr1::bind(
				&object::fruit_removed,
				this,
				std::tr1::placeholders::_1))),
	score_font_node_(
		fruitlib::scenic::parent(
			_overlay,
			fruitlib::scenic::depth(
				depths::overlay::dont_care)),
		fruitlib::font::object_parameters(
			_font_cache.metrics(
				FCPPT_TEXT("score")),
			_font_cache.drawer(
				FCPPT_TEXT("score")),
			SGE_FONT_TEXT_LIT("0"),
			sge::font::rect::null(),
			sge::font::text::align_h::right,
			sge::font::text::align_v::top,
			sge::font::text::flags::none),
		sge::image::color::convert<fruitlib::font::color_format>(
			fruitlib::json::parse_rgba8_color(
				sge::parse::json::find_and_convert_member<sge::parse::json::value>(
					_config_file,
					sge::parse::json::path(
						FCPPT_TEXT("ingame"))
							/FCPPT_TEXT("score-font-color")))),
		fruitlib::font::scale(
			1)),
	timer_font_node_(
		fruitlib::scenic::parent(
			_overlay,
			fruitlib::scenic::depth(
				depths::overlay::dont_care)),
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
		sge::image::color::convert<fruitlib::font::color_format>(
			fruitlib::json::parse_rgba8_color(
				sge::parse::json::find_and_convert_member<sge::parse::json::value>(
					_config_file,
					sge::parse::json::path(
						FCPPT_TEXT("ingame"))
						/ FCPPT_TEXT("timer-font-color")))),
		fruitlib::font::scale(
			1)),
	multiplier_font_node_(
		fruitlib::scenic::parent(
			_overlay,
			fruitlib::scenic::depth(
				depths::overlay::dont_care)),
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
		sge::image::color::any::convert<fruitlib::font::color_format>(
			sge::image::colors::white()),
		fruitlib::font::scale(
			1)),
	score_increase_timer_(
		fruitapp::ingame_timer::parameters(
			_clock,
			fruitlib::time_format::find_and_convert_duration<fruitapp::ingame_timer::duration>(
				_config_file,
				sge::parse::json::path(
					FCPPT_TEXT("ingame"))
					/ FCPPT_TEXT("score-increase-timer")))),
	multiplier_timer_(
		fruitapp::ingame_timer::parameters(
			_clock,
			fruitlib::time_format::find_and_convert_duration<fruitapp::ingame_timer::duration>(
				_config_file,
				sge::parse::json::path(
					FCPPT_TEXT("ingame"))
					/ FCPPT_TEXT("multiplier-timer")))),
	penalty_timer_(
		fruitapp::ingame_timer::parameters(
			_clock,
			fruitlib::time_format::find_and_convert_duration<fruitapp::ingame_timer::duration>(
				_config_file,
				sge::parse::json::path(
					FCPPT_TEXT("ingame"))
					/ FCPPT_TEXT("penalty-timer")))),
	multiplier_(1),
	multi_count_(0),
	renderer_(
		_renderer)
{
	react(
		fruitlib::scenic::events::viewport_change());
}

bool
fruitapp::game_logic::object::finished() const
{
	return
		round_timer_.expired();
}

fruitapp::highscore::score::value_type
fruitapp::game_logic::object::score() const
{
	return
		score_;
}

void
fruitapp::game_logic::object::react(
	fruitlib::scenic::events::update const &)
{
	if (penalty_timer_.active() && penalty_timer_.expired())
	{
		penalty_timer_.reset();
		penalty_timer_.active(
			false);
		multiplier_timer_.reset();
		multiplier_timer_.active(
			true);
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
				fruitlib::font::color_format
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
						sge::timer::elapsed_fractional<float>(multiplier_timer_)))
			(mizuiro::color::init::saturation %= 1.0)
			(mizuiro::color::init::value %= 1.0)
			(mizuiro::color::init::alpha %= 1.0))));
	}
	timer_font_node_.object().text(
		fruitlib::time_format::duration_to_string<sge::font::text::string>(
			sge::timer::remaining<fruitapp::ingame_clock::duration>(
				round_timer_),
			fruitlib::time_format::seconds));

	if(sge::timer::reset_when_expired(score_increase_timer_))
	{
		highscore::score::value_type score_diff = score_ - iterating_score_;
		if (score_diff > 0)
		{
			sound_controller_.play(
				fruitlib::resource_tree::path(FCPPT_TEXT("score_increased")));
			iterating_score_ +=
				(score_diff)/10 + 1;
		}
		score_font_node_.object().text(
			fcppt::lexical_cast<sge::font::text::string>(
				iterating_score_));
	}
}

void
fruitapp::game_logic::object::react(
	fruitlib::scenic::events::viewport_change const &)
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
fruitapp::game_logic::object::fruit_added(
	fruit::object const &)
{
}

void
fruitapp::game_logic::object::fruit_removed(
	fruit::object const &)
{
}

void
fruitapp::game_logic::object::fruit_cut(
	fruit::cut_context const &context)
{
	fruit::tag_set ts = context.old().prototype().tags();
	if(ts.find(FCPPT_TEXT("meat")) != ts.end())
	{
		penalty_timer_.active(
			true);
		penalty_timer_.reset();
		multiplier_timer_.active(
			false);
		multiplier_timer_.reset();
		multiplier_ = 0;
		multi_count_ = 0;
		multiplier_font_node_.scale(
			2);
		multiplier_font_node_.color(
			sge::image::color::any::convert<fruitlib::font::color_format>(
				sge::image::colors::gray()));
	}
	else
		increase_score(
			static_cast<highscore::score::value_type>(
				static_cast<fruit::area::value_type>(
					multiplier_) *
				context.area() *
				area_score_factor_));
	if (!multiplier_timer_.expired())
	{
		++multi_count_;
		if (multi_count_ > 5)
		{
			++multiplier_;
			multi_count_ = 0;
			multiplier_font_node_.scale(
				static_cast<fruitlib::font::scale::value_type>(
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
fruitapp::game_logic::object::increase_score(
	highscore::score::value_type const &s)
{
	score_ +=
		s;
}
