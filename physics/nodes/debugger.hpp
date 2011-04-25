#ifndef FRUITCUT_PHYSICS_NODES_DEBUGGER_HPP_INCLUDED
#define FRUITCUT_PHYSICS_NODES_DEBUGGER_HPP_INCLUDED

#include "../debugger_fwd.hpp"
#include "../../scenic/nodes/intrusive.hpp"
#include <fcppt/noncopyable.hpp>

namespace fruitcut
{
namespace physics
{
namespace nodes
{
class debugger
:
	public scenic::nodes::intrusive
{
FCPPT_NONCOPYABLE(
	debugger);
public:
	explicit
	debugger(
		fruitcut::physics::debugger &);

	~debugger();
private:
	fruitcut::physics::debugger &debugger_;

	void
	update();

	void
	render();
};
}
}
}

#endif
