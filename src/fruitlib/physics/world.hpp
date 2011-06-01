#ifndef FRUITCUT_FRUITLIB_PHYSICS_WORLD_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_PHYSICS_WORLD_HPP_INCLUDED

#include "box.hpp"
#include "vector3.hpp"
#include <sge/time/duration.hpp>
#include <fcppt/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

class btCollisionConfiguration;
class btDispatcher;
class btConstraintSolver;
class btDiscreteDynamicsWorld;
class btBroadphaseInterface;

namespace fruitcut
{
namespace fruitlib
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

	vector3 const
	gravity() const;

	void
	gravity(
		vector3 const &);

	// The debug drawer needs this
	btDiscreteDynamicsWorld &
	handle();

	~world();
private:
	boost::scoped_ptr<btCollisionConfiguration> configuration_;
	boost::scoped_ptr<btDispatcher> dispatcher_;
	boost::scoped_ptr<btBroadphaseInterface> broadphase_;
	boost::scoped_ptr<btConstraintSolver> constraint_solver_;
	boost::scoped_ptr<btDiscreteDynamicsWorld> world_;
};
}
}
}

#endif
