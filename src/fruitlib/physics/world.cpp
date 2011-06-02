#include "world.hpp"
#include "group/object.hpp"
#include "structure_cast.hpp"
#include "rigid_body/collision_data.hpp"
#include "rigid_body/object.hpp"
#include "../exception.hpp"
#include <BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h>
#include <BulletCollision/CollisionDispatch/btCollisionDispatcher.h>
//#include <BulletCollision/BroadphaseCollision/btAxisSweep3.h>
#include <BulletCollision/BroadphaseCollision/btDbvtBroadphase.h>
#include <BulletCollision/CollisionDispatch/btCollisionDispatcher.h>
#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <fcppt/chrono/duration_cast.hpp>
#include <fcppt/chrono/duration_impl.hpp>
#include <fcppt/chrono/milliseconds.hpp>
#include <fcppt/math/box/basic_impl.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/math/vector/arithmetic.hpp>
#include <fcppt/math/vector/dim.hpp>
#include <fcppt/assert.hpp>
#include <fcppt/text.hpp>
#include <boost/static_assert.hpp>
#include <iostream>
#include <limits>

BOOST_STATIC_ASSERT((
	boost::is_same<btScalar,float>::value));

namespace
{
fruitcut::fruitlib::physics::rigid_body::user_data const
void_ptr_to_body(
	void *p)
{
	FCPPT_ASSERT(
		p);

	btCollisionObject *co = 
		static_cast<btCollisionObject *>(
			p);

	FCPPT_ASSERT(
		co->getUserPointer());

	return 
		static_cast<fruitcut::fruitlib::physics::rigid_body::object *>(
			co->getUserPointer())->user_data();
}
}

fruitcut::fruitlib::physics::world::world(
	box const &/*world_size*/,
	vector3 const &_gravity)
:
	configuration_(
		new btDefaultCollisionConfiguration()),
	dispatcher_(
		new btCollisionDispatcher(
			configuration_.get())),
	broadphase_(
		new btDbvtBroadphase()
		/*
		new btAxisSweep3(
			structure_cast(
				world_size.pos()),
			structure_cast(
				world_size.pos() + world_size.size()))*/),
	constraint_solver_(
		new btSequentialImpulseConstraintSolver()),
	world_(
		new btDiscreteDynamicsWorld(
			dispatcher_.get(),
			broadphase_.get(),
			constraint_solver_.get(),
			configuration_.get())),
	next_group_id_(
		static_cast<group::id>(
			1)),
	rigid_body_collision_()
{
	// bullet sets some default value, so we better override this here
	world_->setGravity(
		structure_cast(
			_gravity));
	world_->setInternalTickCallback(
		&internal_tick_callback_static,
		this,
		// isPreTick
		false);
}

void
fruitcut::fruitlib::physics::world::update(
	sge::time::duration const &delta)
{
	world_->stepSimulation(
		static_cast<scalar>(
			fcppt::chrono::duration_cast<fcppt::chrono::milliseconds>(
				delta).count())/
		static_cast<scalar>(
			1000),
		// Maximum simulation substeps: The fixed time step is 1/60. We
		// have to choose the substep count so that time_delta is _just_
		// less than the fixed time.
		3);
}

fruitcut::fruitlib::physics::vector3 const
fruitcut::fruitlib::physics::world::gravity() const
{
	return 
		structure_cast<vector3>(
			world_->getGravity());
}

void
fruitcut::fruitlib::physics::world::gravity(
	vector3 const &_gravity)
{
	world_->setGravity(
		structure_cast(
			_gravity));
}

btDiscreteDynamicsWorld &
fruitcut::fruitlib::physics::world::handle()
{
	return *world_;
}

void
fruitcut::fruitlib::physics::world::add_body(
	rigid_body::object &_body,
	group::sequence const &_groups)
{
	group::id 
		group = static_cast<group::id>(
			0),
		mask = group;

	for(group::sequence::const_iterator r = _groups.begin(); r != _groups.end(); ++r)
	{
		group |= r->get().category_;
		mask |= r->get().collides_;
	}

	world_->addRigidBody(
		_body.body_.get(),
		group,
		mask);
}

void
fruitcut::fruitlib::physics::world::remove_body(
	rigid_body::object &_body)
{
	world_->removeRigidBody(
		_body.body_.get());
}

void
fruitcut::fruitlib::physics::world::make_groups_collide(
	group::object &a,
	group::object &b)
{
	a.collides_with(
		b);
	b.collides_with(
		a);
}

fruitcut::fruitlib::physics::group::id
fruitcut::fruitlib::physics::world::next_group_id()
{
	if(next_group_id_ == std::numeric_limits<group::id>::max())
		throw fruitlib::exception(
			FCPPT_TEXT("You have created too many groups. Bummer. :/"));
	group::id const old = next_group_id_;
	// <<= doesn't work because of -Wconversion
	next_group_id_ = 
		static_cast<group::id>(
			next_group_id_ << 1);
	return old;
}

fcppt::signal::auto_connection
fruitcut::fruitlib::physics::world::rigid_body_collision(
	rigid_body::collision_callback const &f)
{
	return 
		rigid_body_collision_.connect(
			f);
}

fruitcut::fruitlib::physics::world::~world()
{
}

void
fruitcut::fruitlib::physics::world::internal_tick_callback_static(
	btDynamicsWorld *w,
	btScalar const time_step)
{
	FCPPT_ASSERT(
		w->getWorldUserInfo());

	static_cast<physics::world *>(
		w->getWorldUserInfo())->internal_tick_callback(
		time_step);
}

void
fruitcut::fruitlib::physics::world::internal_tick_callback(
	btScalar /* time_step */)
{
	for(
		btPersistentManifold **current_manifold =
			world_->getDispatcher()->getInternalManifoldPointer()
		,
		**manifold_end =
				world_->getDispatcher()->getInternalManifoldPointer() + 
				world_->getDispatcher()->getNumManifolds()
		;
		current_manifold != manifold_end;
		++current_manifold
	)
	{
		if(!(*current_manifold)->getNumContacts())
			continue;

		rigid_body_collision_(
			rigid_body::collision_data(
				::void_ptr_to_body(
					(*current_manifold)->getBody0()),
				::void_ptr_to_body(
					(*current_manifold)->getBody1())));

		// For additional data:
		/*
		int numContacts = contactManifold->getNumContacts();
		for (int j=0;j<numContacts;j++)
		{
			btManifoldPoint& pt = contactManifold->getContactPoint(j);
			if (pt.getDistance()<0.f)
			{
				const btVector3& ptA = pt.getPositionWorldOnA();
				const btVector3& ptB = pt.getPositionWorldOnB();
				const btVector3& normalOnB = pt.m_normalWorldOnB;
			}
		}
		*/
	}
}
