#ifndef FRUITCUT_APP_STATES_INGAME_HPP_INCLUDED
#define FRUITCUT_APP_STATES_INGAME_HPP_INCLUDED

#include "../machine.hpp"
#include "running_fwd.hpp"
#include "../fruit_prototype.hpp"
#include "../fruit.hpp"
#include "../../physics/world.hpp"
#include <sge/shader/object.hpp>
#include <sge/camera/object.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <boost/statechart/state.hpp>
#include <boost/ptr_container/ptr_list.hpp>
#include <vector>

namespace fruitcut
{
namespace app
{
namespace states
{
class ingame
:
	// The second argument has to be a complete type, the third one
	// doesn't have to be
	public boost::statechart::state<ingame,machine,running>
{
public:
	explicit
	ingame(
		my_context);

	physics::world &
	physics_world();

	physics::world const &
	physics_world() const;

	void
	render_fruits();

	sge::camera::object &
	camera();

	sge::camera::object const &
	camera() const;

	~ingame();
private:
	typedef
	std::vector<fruit_prototype>
	prototype_sequence;

	typedef
	boost::ptr_list<fruit>
	fruit_sequence;

	fcppt::signal::scoped_connection toggle_pause_connection_;
	sge::camera::object camera_;
	prototype_sequence prototypes_;
	physics::world physics_world_;
	fruit_sequence fruits_; 
	sge::shader::object fruit_shader_;
};
}
}
}

#endif
