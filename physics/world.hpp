#ifndef FRUITCUT_PHYSICS_WORLD_HPP_INCLUDED
#define FRUITCUT_PHYSICS_WORLD_HPP_INCLUDED

#include "box.hpp"
#include "vector3.hpp"
#include <fcppt/noncopyable.hpp>
#include <fcppt/scoped_ptr.hpp>
#include <sge/time/duration.hpp>

class btCollisionConfiguration;
class btDispatcher;
class btConstraintSolver;
class btDiscreteDynamicsWorld;
class btBroadphaseInterface;

namespace fruitcut
{
namespace physics
{
class world
{
FCPPT_NONCOPYABLE(
	world);
public:
	explicit
	world(
		box const &world_size,
		vector3 const &gravity);

	void
	update(
		sge::time::duration const &);

	// The debug drawer needs this
	btDiscreteDynamicsWorld &
	handle();

	~world();
private:
	fcppt::scoped_ptr<btCollisionConfiguration> configuration_;
	fcppt::scoped_ptr<btDispatcher> dispatcher_;
	fcppt::scoped_ptr<btBroadphaseInterface> broadphase_;
	fcppt::scoped_ptr<btConstraintSolver> constraint_solver_;
	fcppt::scoped_ptr<btDiscreteDynamicsWorld> world_;
};
}
}

#endif
