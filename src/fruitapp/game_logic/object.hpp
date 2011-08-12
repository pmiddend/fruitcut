#ifndef FRUITAPP_GAME_LOGIC_OBJECT_HPP_INCLUDED
#define FRUITAPP_GAME_LOGIC_OBJECT_HPP_INCLUDED

#include <fruitapp/overlay.hpp>
#include <fruitapp/bonsu/manager.hpp>
#include <fruitapp/bonsu/instance/gravity.hpp>
#include <fruitapp/ingame_clock.hpp>
#include <fruitapp/ingame_timer.hpp>
#include <fruitapp/fruit/manager_fwd.hpp>
#include <fruitapp/fruit/object_fwd.hpp>
#include <fruitapp/fruit/cut_context_fwd.hpp>
#include <fruitapp/fruit/area.hpp>
#include <fruitlib/font/cache_fwd.hpp>
#include <fruitlib/font/scene_node.hpp>
#include <fruitlib/physics/world_fwd.hpp>
#include <fruitlib/audio/sound_controller.hpp>
#include <fruitlib/scenic/node.hpp>
#include <fruitlib/scenic/parent_fwd.hpp>
#include <fruitlib/scenic/events/update.hpp>
#include <fruitlib/scenic/events/viewport_change.hpp>
#include <fruitapp/highscore/score.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/renderer/device_fwd.hpp>
#include <sge/image2d/multi_loader_fwd.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <fcppt/noncopyable.hpp>
#include <boost/mpl/vector/vector10.hpp>

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
	boost::mpl::vector2<fruitlib::scenic::events::update,fruitlib::scenic::events::viewport_change>
	scene_reactions;

	explicit
	object(
		fruitlib::scenic::optional_parent const &,
		fruitapp::ingame_clock const &,
		// to get round seconds and stuff
		sge::parse::json::object const &,
		// to get 
		// - "fruit was cut"
		// - "fruit was deleted" 
		// - "fruit was added" (we could consult the spawner for that, but that's not The Right Thing)
		fruitlib::audio::sound_controller &,
		fruit::manager &,
		fruitlib::physics::world &,
		fruitlib::font::cache &,
		overlay &,
		sge::renderer::device &,
		sge::image2d::multi_loader &);

	bool 
	finished() const;

	highscore::score::value_type
	score() const;

	void
	react(
		fruitlib::scenic::events::update const &);

	void
	react(
		fruitlib::scenic::events::viewport_change const &);
private:
	sge::renderer::device &renderer_;
	bonsu::manager bonsu_manager_;
	fruit::area::value_type const area_score_factor_;
	highscore::score::value_type score_,iterating_score_;
	fruitapp::ingame_timer round_timer_;
	fruitlib::audio::sound_controller &sound_controller_;
	fcppt::signal::scoped_connection fruit_added_connection_;
	fcppt::signal::scoped_connection fruit_cut_connection_;
	fcppt::signal::scoped_connection fruit_removed_connection_;
	fruitlib::font::scene_node score_font_node_;
	fruitlib::font::scene_node timer_font_node_;
	fruitlib::font::scene_node multiplier_font_node_;
	fruitapp::ingame_timer score_increase_timer_;
	fruitapp::ingame_timer multiplier_timer_;
	fruitapp::ingame_timer penalty_timer_;
	int multiplier_;
	unsigned multi_count_;
	fruitapp::bonsu::instance::gravity gravity_bonsu_;

	void
	fruit_added(
		fruit::object const &);

	void
	fruit_removed(
		fruit::object const &);

	void
	fruit_cut(
		fruit::cut_context const &);

	void
	increase_score(
		highscore::score::value_type const &);
};
}
}

#endif
