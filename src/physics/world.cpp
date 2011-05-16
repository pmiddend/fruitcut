#include "world.hpp"
#include "structure_cast.hpp"
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
#include <iostream>

fruitcut::physics::world::world(
	box const &/*world_size*/,
	vector3 const &gravity)
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
			configuration_.get()))
{
	// bullet sets some default value, so we better override this here
	world_->setGravity(
		structure_cast(
			gravity));
}

void
fruitcut::physics::world::update(
	sge::time::duration const &delta)
{
	world_->stepSimulation(
		static_cast<scalar>(
			fcppt::chrono::duration_cast<fcppt::chrono::milliseconds>(
				delta).count())/
		static_cast<scalar>(
			1000)/5,
		// Maximum simulation substeps: The fixed time step is 1/60. We
		// have to choose the substep count so that time_delta is _just_
		// less than the fixed time.
		3);
}

btDiscreteDynamicsWorld &
fruitcut::physics::world::handle()
{
	return *world_;
}

fruitcut::physics::world::~world()
{
}