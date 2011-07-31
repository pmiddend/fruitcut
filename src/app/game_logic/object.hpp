#ifndef FRUITCUT_APP_GAME_LOGIC_OBJECT_HPP_INCLUDED
#define FRUITCUT_APP_GAME_LOGIC_OBJECT_HPP_INCLUDED

#include "../overlay.hpp"
#include "../fruit/manager_fwd.hpp"
#include "../fruit/object_fwd.hpp"
#include "../fruit/cut_context_fwd.hpp"
#include "../fruit/area.hpp"
#include "../../fruitlib/font/cache_fwd.hpp"
#include "../../fruitlib/font/scene_node.hpp"
#include "../../fruitlib/scenic/node.hpp"
#include "../../fruitlib/scenic/parent_fwd.hpp"
#include "../../fruitlib/scenic/events/update_fwd.hpp"
#include "../highscore/score.hpp"
#include <sge/parse/json/object_fwd.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/renderer/device_fwd.hpp>
#include <sge/time/callback.hpp>
#include <sge/time/timer.hpp>
#include <sge/viewport/manager_fwd.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <fcppt/noncopyable.hpp>
#include <boost/mpl/vector/vector10.hpp>

namespace fruitcut
{
namespace app
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

	explicit
	object(
		fruitlib::scenic::optional_parent const &,
		sge::time::callback const &,
		// to get round seconds and stuff
		sge::parse::json::object const &,
		// to get 
		// - "fruit was cut"
		// - "fruit was deleted" 
		// - "fruit was added" (we could consult the spawner for that, but that's not The Right Thing)
		fruit::manager &,
		fruitlib::font::cache &,
		overlay &,
		sge::renderer::device &,
		sge::viewport::manager &);

	bool 
	finished() const;

	highscore::score::value_type
	score() const;

	void
	react(
		fruitlib::scenic::events::update const &);
private:
	fruit::area::value_type const area_score_factor_;
	highscore::score::value_type score_,iterating_score_;
	sge::time::timer round_timer_;
	fcppt::signal::scoped_connection 
		fruit_added_connection_,
		fruit_cut_connection_,
		fruit_removed_connection_,
		viewport_changed_connection_;
	fruitlib::font::scene_node score_font_node_;
	fruitlib::font::scene_node timer_font_node_;
	fruitlib::font::scene_node multiplier_font_node_;
	sge::time::timer score_increase_timer_;
	sge::time::timer multiplier_timer_;
	sge::time::timer penalty_timer_;
	int multiplier_;
	unsigned multi_count_;
	sge::renderer::device &renderer_;

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
	viewport_changed();


	void
	increase_score(
		highscore::score::value_type const &);
};
}
}
}

#endif
