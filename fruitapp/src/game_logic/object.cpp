#include <fruitapp/depths/overlay.hpp>
#include <fruitapp/fruit/cut_context.hpp>
#include <fruitapp/fruit/manager.hpp>
#include <fruitapp/game_logic/object.hpp>
#include <fruitapp/quick_log.hpp>
#include <fruitapp/projection_manager/object.hpp>
#include <fruitapp/viewport/manager.hpp>
#include <fruitlib/font/cache.hpp>
#include <fruitlib/font/object.hpp>
#include <fruitlib/font/object_parameters.hpp>
#include <fruitlib/json/parse_rgba8_color.hpp>
#include <fruitlib/resource_tree/path.hpp>
#include <fruitlib/time_format/duration_to_string.hpp>
#include <fruitlib/time_format/find_and_convert_duration.hpp>
#include <fruitlib/time_format/milliseconds.hpp>
#include <fruitlib/time_format/seconds.hpp>
#include <sge/font/dim.hpp>
#include <sge/font/lit.hpp>
#include <sge/font/object.hpp>
/*
#include <sge/font/text.hpp>
#include <sge/font/text_parameters.hpp>
*/
#include <sge/font/vector.hpp>
#include <sge/image/colors.hpp>
#include <sge/image/color/init.hpp>
#include <sge/image/color/rgba8.hpp>
#include <sge/image/color/rgba8_format.hpp>
#include <sge/image/color/any/convert.hpp>
#include <sge/parse/json/find_and_convert_member.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <sge/parse/json/value.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/renderer/device/ffp.hpp>
#include <sge/renderer/target/onscreen.hpp>
#include <sge/timer/elapsed_fractional.hpp>
#include <sge/timer/parameters.hpp>
#include <sge/timer/remaining.hpp>
#include <sge/timer/remaining_fractional.hpp>
#include <sge/timer/reset_when_expired.hpp>
#include <mizuiro/color/convert.hpp>
#include <mizuiro/color/homogenous_static.hpp>
#include <mizuiro/color/init.hpp>
#include <mizuiro/color/object.hpp>
#include <mizuiro/color/convert_static/converter.hpp>
#include <mizuiro/color/layout/hsva.hpp>
#include <fcppt/format.hpp>
#include <fcppt/insert_to_std_wstring.hpp>
#include <fcppt/insert_to_string.hpp>
#include <fcppt/string.hpp>
#include <fcppt/text.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/math/clamp.hpp>
#include <fcppt/math/box/object_impl.hpp>
#include <fcppt/math/box/output.hpp>
#include <fcppt/math/dim/arithmetic.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/tr1/functional.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/cstdint.hpp>
#include <boost/format.hpp>
#include <boost/chrono/duration.hpp>
#include <iostream>
#include <fcppt/config/external_end.hpp>


/*
namespace
{
void
reset_text_and_center(
	sge::font::string const &_string,
	sge::renderer::vector2 const &_position,
	fruitlib::font::object &_object)
{
	sge::font::rect const calculated_rect(
		_object.font_object().create_text(
			_string,
			sge::font::text_parameters(
				sge::font::align_h::left))->rect());

	_object.text(
		_string);

	_object.bounding_box(
		sge::font::rect(
			sge::font::rect::vector(
				static_cast<sge::font::unit>(
					_position.x() -
					static_cast<sge::renderer::scalar>(
						calculated_rect.size().w()/2) +
					static_cast<sge::renderer::scalar>(
						calculated_rect.pos().x())),
				static_cast<sge::font::unit>(
					_position.y() -
					static_cast<sge::renderer::scalar>(
						calculated_rect.size().h()/2) +
					static_cast<sge::renderer::scalar>(
						calculated_rect.pos().y()))),
			2*calculated_rect.size()));
}
}
*/

fruitapp::game_logic::object::object(
	fruitlib::scenic::optional_parent const &_parent,
	fruitapp::projection_manager::object &_projection_manager,
	fruitapp::ingame_clock const &_clock,
	// to get round seconds and stuff
	sge::parse::json::object const &_config_file,
	// to get
	// - "fruit was cut"
	// - "fruit was deleted"
	// - "fruit was added" (we could consult the spawner for that, but that's not The Right Thing)
	fruitlib::audio::sound_controller &_sound_controller,
	fruitapp::fruit::manager &_fruit_manager,
	fruitlib::font::manager &_font_manager,
	fruitapp::overlay &_overlay,
	fruitapp::viewport::manager &_viewport_manager,
	fruitapp::quick_log &_log)
:
	node_base(
		fruitlib::scenic::optional_parent(
			_parent)),
	projection_manager_(
		_projection_manager),
	area_score_factor_(
		sge::parse::json::find_and_convert_member<fruitapp::fruit::area::value_type>(
			_config_file,
			sge::parse::json::path(
				FCPPT_TEXT("ingame"))
				/ FCPPT_TEXT("area-score-factor"))),
	score_(
		0u),
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
	font_particles_(
		fruitlib::scenic::optional_parent(
			fruitlib::scenic::parent(
				_overlay,
				fruitlib::scenic::depth(
					depths::overlay::dont_care))),
		_projection_manager,
		_clock,
		_font_manager),
	score_font_node_(
		fruitlib::scenic::optional_parent(
			fruitlib::scenic::parent(
				_overlay,
				fruitlib::scenic::depth(
					depths::overlay::dont_care))),
		fruitlib::font::object_parameters(
			_font_manager,
			fruitlib::font::identifier(
				fcppt::string(
					FCPPT_TEXT("score"))),
			sge::font::string(
				SGE_FONT_LIT("0")),
			sge::font::rect::null(),
			sge::font::align_h::right,
			fruitlib::font::align_v::top,
			sge::font::flags_field::null(),
			sge::image::color::any::object(
				fruitlib::json::parse_rgba8_color(
					sge::parse::json::find_and_convert_member<sge::parse::json::value const>(
						_config_file,
						sge::parse::json::path(
							FCPPT_TEXT("ingame"))
								/FCPPT_TEXT("score-font-color")))),
			fruitlib::font::scale(
				1.f))),
	timer_font_node_(
		fruitlib::scenic::optional_parent(
			fruitlib::scenic::parent(
				_overlay,
				fruitlib::scenic::depth(
					depths::overlay::dont_care))),
		fruitlib::font::object_parameters(
			_font_manager,
			fruitlib::font::identifier(
				fcppt::string(
					FCPPT_TEXT("score"))),
			sge::font::string(
				SGE_FONT_LIT("0")),
			sge::font::rect::null(),
			sge::font::align_h::center,
			fruitlib::font::align_v::top,
			sge::font::flags_field::null(),
			sge::image::color::any::object(
				fruitlib::json::parse_rgba8_color(
					sge::parse::json::find_and_convert_member<sge::parse::json::value const>(
						_config_file,
						sge::parse::json::path(
							FCPPT_TEXT("ingame"))
							/ FCPPT_TEXT("timer-font-color")))),
			fruitlib::font::scale(
				1.0f))),
	multiplier_font_node_(
		fruitlib::scenic::optional_parent(
			fruitlib::scenic::parent(
				_overlay,
				fruitlib::scenic::depth(
					depths::overlay::dont_care))),
		fruitlib::font::object_parameters(
			_font_manager,
			fruitlib::font::identifier(
				fcppt::string(
					FCPPT_TEXT("score"))),
			sge::font::string(
				SGE_FONT_LIT("")),
			sge::font::rect::null(),
			sge::font::align_h::center,
			fruitlib::font::align_v::bottom,
			sge::font::flags_field::null(),
			sge::image::colors::white(),
			fruitlib::font::scale(
				1.f))),
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
	viewport_change_connection_(
		_viewport_manager.change_callback(
			std::tr1::bind(
				&game_logic::object::viewport_change,
				this,
				std::tr1::placeholders::_1),
			fruitapp::viewport::trigger_early(
				true))),
	cut_fruits_(),
	quick_log_(
		_log)
{
}

bool
fruitapp::game_logic::object::finished() const
{
	return
		round_timer_.expired();
}

fruitapp::highscore::score const
fruitapp::game_logic::object::score() const
{
	return
		score_;
}

void
fruitapp::game_logic::object::react(
	fruitlib::scenic::events::update const &)
{
	font_particles_.update();

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
				SGE_FONT_LIT(""));
	}
	if (multiplier_timer_.active() && multiplier_timer_.expired())
	{
		multiplier_ = 1;
		multi_count_ = 0;
		multiplier_timer_.reset();
		multiplier_font_node_.object().text(
				SGE_FONT_LIT(""));
	}
	else if (multiplier_timer_.active())
	{
		typedef
		mizuiro::color::object
		<
			mizuiro::color::homogenous_static
			<
				boost::uint8_t,
				mizuiro::color::layout::hsva
			>
		>
		hsva_color;

		multiplier_font_node_.object().color(
			sge::image::color::any::object(
				mizuiro::color::convert<mizuiro::color::convert_static::converter,sge::image::color::rgba8_format>(
					hsva_color(
						(mizuiro::color::init::hue() %=
								0.34 *
								(1. -
									sge::timer::elapsed_fractional<double>(multiplier_timer_)))
						(mizuiro::color::init::saturation() %= 1.0)
						(mizuiro::color::init::value() %= 1.0)
						(mizuiro::color::init::alpha() %= 1.0)))));
	}
	unsigned const seconds_remaining =
		static_cast<unsigned>(
			sge::timer::remaining<boost::chrono::seconds>(
				round_timer_).count());

	unsigned const minutes_remaining =
		seconds_remaining / 60u;

	sge::font::string const timer_text(
		minutes_remaining
		?
			(boost::wformat(
				SGE_FONT_LIT("%02d:%02d")) % minutes_remaining % (seconds_remaining % 60u)).str()
		:
			(boost::wformat(
				SGE_FONT_LIT("%02d")) % seconds_remaining).str());

	timer_font_node_.object().text(
		timer_text);
	/*
	timer_font_node_.object().text(
		fruitlib::time_format::duration_to_string<sge::font::string>(
			sge::timer::remaining<fruitapp::ingame_clock::duration>(
				round_timer_),
			fruitlib::time_format::seconds));
	*/

	if(sge::timer::reset_when_expired(score_increase_timer_))
	{
		fruitapp::highscore::score score_diff(
			score_ - iterating_score_);
		if (score_diff.get() > 0)
		{
			/*
			sound_controller_.play(
				fruitlib::resource_tree::path(FCPPT_TEXT("score_increased")));
			*/
			iterating_score_ +=
				(score_diff)/fruitapp::highscore::score(10u) + fruitapp::highscore::score(1u);
		}
		score_font_node_.object().text(
			fcppt::insert_to_string<sge::font::string>(
				iterating_score_.get()));
	}
}

void
fruitapp::game_logic::object::viewport_change(
	sge::renderer::target::viewport const &_viewport)
{
	sge::font::dim const &viewport_dim =
		fcppt::math::dim::structure_cast<sge::font::dim>(
			_viewport.get().size());

	score_font_node_.object().bounding_box(
		sge::font::rect(
			sge::font::vector::null(),
			viewport_dim));

	multiplier_font_node_.object().bounding_box(
		sge::font::rect(
			sge::font::vector::null(),
			sge::font::dim(
				viewport_dim.w(),
				static_cast<sge::font::unit>(
					static_cast<sge::renderer::scalar>(
						viewport_dim.h()) *
					static_cast<sge::renderer::scalar>(
						0.9)))));

	timer_font_node_.object().bounding_box(
		sge::font::rect(
			sge::font::vector::null(),
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
	fruit::object const &_fruit)
{
	cut_fruits_.erase(
		&_fruit);
}

void
fruitapp::game_logic::object::fruit_cut(
	fruitapp::fruit::cut_context const &_context)
{
	cut_fruits_container::iterator old_fruit =
		cut_fruits_.end();

	unsigned parent =
		((old_fruit = cut_fruits_.find( &_context.old())) != cut_fruits_.end())
		?
			old_fruit->second
		:
			0
	;

	for (
		fruitapp::fruit::cut_context::new_fruit_array::const_iterator it =
			_context.new_fruits().cbegin();
		it !=
			_context.new_fruits().cend();
		++it)
	{
		cut_fruits_.insert(
			cut_fruits_container::value_type(
				*it,
				parent + 1
				)
		);
	}

	fruitapp::fruit::tag_set ts = _context.old().prototype().tags();

	if(ts.find(FCPPT_TEXT("meat")) != ts.end())
	{
		sound_controller_.play(
			fruitlib::resource_tree::path(FCPPT_TEXT("penalty")));
		penalty_timer_.active(
			true);
		penalty_timer_.reset();
		multiplier_timer_.active(
			false);
		multiplier_timer_.reset();
		multiplier_ = 0;
		multi_count_ = 0;
		multiplier_font_node_.object().scale(
			fruitlib::font::scale(
				2.0f));
		multiplier_font_node_.object().color(
			sge::image::colors::gray());
	}
	else
	{
		fruitapp::highscore::score const increment(
			static_cast<fruitapp::highscore::score::value_type>(
				static_cast<fruitapp::fruit::area::value_type>(
					multiplier_) *
				_context.area().get() *
				area_score_factor_));

		font_particles_.add_particle(
			fruitapp::font_particle::text(
				fcppt::insert_to_std_wstring(
					increment.get())),
			fruitlib::font::identifier(
				fcppt::string(
					FCPPT_TEXT("score"))),
			fruitapp::font_particle::position(
				projection_manager_.project_point(
					_context.old().position())),
			fruitapp::font_particle::lifetime(
				2.0f),
			sge::image::colors::white());
		/*
		reset_text_and_center(
			fcppt::insert_to_std_wstring(
				increment.get()),
			projection_manager_.project_point(
				_context.old().position()),
			score_increase_node_.object());
		*/

		this->increase_score(
			increment);
	}

	if (!multiplier_timer_.expired())
	{
		++multi_count_;
		if (multi_count_ > 5)
		{
			++multiplier_;
			multi_count_ = 0;
			multiplier_font_node_.object().scale(
				fruitlib::font::scale(
					0.75f *
					std::sqrt(static_cast<float>(multiplier_ + 1))));
		}
		multiplier_timer_.reset();
		if (multiplier_ != 1)
			multiplier_font_node_.object().text(
					fcppt::insert_to_string<sge::font::string>(
						multiplier_) +
					SGE_FONT_LIT("x"));
	}
}

void
fruitapp::game_logic::object::increase_score(
	fruitapp::highscore::score const &s)
{
	score_ +=
		s;
}
