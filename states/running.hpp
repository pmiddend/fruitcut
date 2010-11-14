#ifndef FRUITCUT_STATES_RUNNING_HPP_INCLUDED
#define FRUITCUT_STATES_RUNNING_HPP_INCLUDED

#include "../machine.hpp"
#include "../mesh.hpp"
#include "../events/tick.hpp"
#include "../events/render.hpp"
#include <sge/renderer/vertex_buffer_ptr.hpp>
#include <sge/shader/object.hpp>
#include <sge/model/object_ptr.hpp>
#include <sge/renderer/state/scoped.hpp>
#include <boost/statechart/state.hpp>
#include <boost/statechart/custom_reaction.hpp>
#include <boost/mpl/vector/vector10.hpp>

namespace fruitcut
{
namespace states
{
class running
:
	public boost::statechart::state<running,machine>
{
public:
	typedef 
	boost::mpl::vector2
	<
		boost::statechart::custom_reaction<events::tick>,
		boost::statechart::custom_reaction<events::render>
	> 
	reactions;

	explicit
	running(
		my_context);

	boost::statechart::result
	react(
		events::tick const &);

	boost::statechart::result
	react(
		events::render const &);
private:
	sge::shader::object shader_;
	sge::model::object_ptr model_;
	mesh mesh_;
	sge::renderer::vertex_buffer_ptr mesh_vb_;
	sge::renderer::state::scoped scoped_state_;
};
}
}

#endif
