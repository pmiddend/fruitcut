#ifndef FRUITAPP_GAME_LOGIC_OBJECT_HPP_INCLUDED
#define FRUITAPP_GAME_LOGIC_OBJECT_HPP_INCLUDED

#include <fruitapp/ingame_clock.hpp>
#include <fruitapp/ingame_timer.hpp>
#include <fruitapp/overlay.hpp>
#include <fruitapp/quick_log.hpp>
#include <fruitapp/font_particle/manager.hpp>
#include <fruitapp/fruit/area.hpp>
#include <fruitapp/fruit/cut_context_fwd.hpp>
#include <fruitapp/fruit/manager_fwd.hpp>
#include <fruitapp/fruit/object_fwd.hpp>
#include <fruitapp/highscore/score.hpp>
#include <fruitapp/projection_manager/object_fwd.hpp>
#include <fruitapp/viewport/manager_fwd.hpp>
#include <fruitlib/audio/sound_controller.hpp>
#include <fruitlib/font/manager_fwd.hpp>
#include <fruitlib/font/scene_node.hpp>
#include <fruitlib/scenic/node.hpp>
#include <fruitlib/scenic/parent_fwd.hpp>
#include <fruitlib/scenic/events/update.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/renderer/target/viewport.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <map>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
namespace game_logic
{
class object
:
	public fruitlib::scenic::node<object>
{
FCPPT_NONCOPYABLE(
	object);
public:
	typedef
	boost::mpl::vector1<fruitlib::scenic::events::update>
	scene_reactions;

	object(
		fruitlib::scenic::optional_parent const &,
		fruitapp::projection_manager::object &,
		fruitapp::ingame_clock const &,
		// to get round seconds and stuff
		sge::parse::json::object const &,
		// to get
		// - "fruit was cut"
		// - "fruit was deleted"
		// - "fruit was added" (we could consult the spawner for that, but that's not The Right Thing)
		fruitlib::audio::sound_controller &,
		fruitapp::fruit::manager &,
		fruitlib::font::manager &,
		fruitapp::overlay &,
		fruitapp::viewport::manager &,
		fruitapp::quick_log &_log);

	bool
	finished() const;

	fruitapp::highscore::score const
	score() const;

	void
	react(
		fruitlib::scenic::events::update const &);
private:
	fruitapp::projection_manager::object const &projection_manager_;
	fruitapp::fruit::area::value_type const area_score_factor_;
	fruitapp::highscore::score score_,iterating_score_;
	fruitapp::ingame_timer round_timer_;
	fruitlib::audio::sound_controller &sound_controller_;
	fcppt::signal::scoped_connection const fruit_added_connection_;
	fcppt::signal::scoped_connection const fruit_cut_connection_;
	fcppt::signal::scoped_connection const fruit_removed_connection_;
	fruitapp::font_particle::manager font_particles_;
	fruitlib::font::scene_node score_font_node_;
	fruitlib::font::scene_node timer_font_node_;
	fruitlib::font::scene_node multiplier_font_node_;
	fruitapp::ingame_timer score_increase_timer_;
	fruitapp::ingame_timer multiplier_timer_;
	fruitapp::ingame_timer penalty_timer_;
	int multiplier_;
	unsigned multi_count_;
	fcppt::signal::scoped_connection viewport_change_connection_;
	typedef std::map<fruit::object const *, unsigned> cut_fruits_container;
	cut_fruits_container cut_fruits_;
	fruitapp::quick_log &quick_log_;

	void
	fruit_added(
		fruitapp::fruit::object const &);

	void
	fruit_removed(
		fruitapp::fruit::object const &);

	void
	fruit_cut(
		fruitapp::fruit::cut_context const &);

	void
	increase_score(
		fruitapp::highscore::score const &);

	void
	viewport_change(
		sge::renderer::target::viewport const &);
};
}
}

#endif
