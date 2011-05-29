#ifndef FRUITCUT_APP_GAME_LOGIC_HPP_INCLUDED
#define FRUITCUT_APP_GAME_LOGIC_HPP_INCLUDED

#include "overlay.hpp"
#include "fruit/manager_fwd.hpp"
#include "fruit/object_fwd.hpp"
#include "fruit/cut_context_fwd.hpp"
#include "fruit/area.hpp"
#include "../font/intrusive_scene_node.hpp"
#include "../font/cache_fwd.hpp"
#include "../scenic/nodes/intrusive.hpp"
#include "score.hpp"
#include <sge/parse/json/object_fwd.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/renderer/device_fwd.hpp>
#include <sge/time/callback.hpp>
#include <sge/time/timer.hpp>
#include <sge/viewport/manager_fwd.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <fcppt/noncopyable.hpp>

namespace fruitcut
{
namespace app
{
class game_logic
:
	public scenic::nodes::intrusive
{
FCPPT_NONCOPYABLE(
	game_logic);
public:
	explicit
	game_logic(
		sge::time::callback const &,
		// to get round seconds and stuff
		sge::parse::json::object const &,
		// to get 
		// - "fruit was cut"
		// - "fruit was deleted" 
		// - "fruit was added" (we could consult the spawner for that, but that's not The Right Thing)
		fruit::manager &,
		font::cache &,
		overlay &,
		sge::renderer::device &,
		sge::viewport::manager &);

	bool 
	finished() const;

	fruitcut::app::score::value_type
	score() const;
private:
	fruit::area::value_type const area_score_factor_;
	fruitcut::app::score::value_type score_,iterating_score_;
	sge::time::timer round_timer_;
	fcppt::signal::scoped_connection 
		fruit_added_connection_,
		fruit_cut_connection_,
		fruit_removed_connection_,
		viewport_changed_connection_;
	font::intrusive_scene_node score_font_node_;
	font::intrusive_scene_node timer_font_node_;
	sge::time::timer score_increase_timer_;
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
	update();

	void
	render();

	void
	increase_score(
		fruitcut::app::score const &);
};
}
}

#endif
