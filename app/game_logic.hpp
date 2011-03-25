#ifndef FRUITCUT_APP_GAME_LOGIC_HPP_INCLUDED
#define FRUITCUT_APP_GAME_LOGIC_HPP_INCLUDED

#include "fruit/manager_fwd.hpp"
#include "fruit/object_fwd.hpp"
#include "../font/system_fwd.hpp"
#include "../font/particle/animated.hpp"
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
		font::system &,
		sge::renderer::device &,
		sge::viewport::manager &);

	bool 
	finished() const;

	fruitcut::app::score
	score() const;

	void
	update();
private:
	fruitcut::app::score score_;
	sge::time::timer round_timer_;
	fcppt::signal::scoped_connection 
		fruit_added_connection_,
		fruit_cut_connection_,
		fruit_removed_connection_,
		viewport_changed_connection_;
	font::particle::animated score_font_;
	font::particle::animated timer_font_;
	sge::time::timer timer_update_timer_;
	sge::renderer::device &renderer_;

	void
	fruit_added(
		fruit::object const &);

	void
	fruit_removed(
		fruit::object const &);

	void
	fruit_cut(
		fruit::object const &old,
		fruit::object const &new_1,
		fruit::object const &new_2,
		sge::renderer::scalar);

	void
	viewport_changed();
};
}
}

#endif