#include "object.hpp"
#include "parameters.hpp"
#include "../scalar.hpp"
#include "../matrix4_to_matrix3.hpp"
#include "../matrix3_to_matrix4.hpp"
#include "../matrix3.hpp"
#include "../matrix4.hpp"
#include "../vector3.hpp"
#include "../structure_cast.hpp"
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <LinearMath/btMotionState.h>
#include <LinearMath/btVector3.h>
#include <LinearMath/btTransform.h>
#include <LinearMath/btDefaultMotionState.h>
#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include <fcppt/math/matrix/basic_impl.hpp>
#include <fcppt/math/matrix/arithmetic.hpp>
#include <fcppt/math/matrix/translation.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/optional.hpp>

namespace
{
btRigidBody::btRigidBodyConstructionInfo const
create_construction_info(
	btMotionState *motion_state,
	btCollisionShape *shape,
	fcppt::optional<fruitcut::fruitlib::physics::scalar> const &mass)
{
	btVector3 inertia(0,0,0);
	if (mass)
		shape->calculateLocalInertia(
			*mass,
			inertia);
	return 
		btRigidBody::btRigidBodyConstructionInfo(
				mass 
				? 
					*mass 
				: 
					static_cast<btScalar>(0),
				motion_state,
				shape,
				inertia);
}
}

fruitcut::fruitlib::physics::rigid_body::object::object(
	parameters const &params)
:
	shape_(
		params.shape()),
	motion_state_(
		fcppt::make_unique_ptr<btDefaultMotionState>(
			btTransform(
				structure_cast(
					matrix4_to_matrix3<scalar>(
						params.transformation())),
				structure_cast(
					params.position())))),
	body_(
		fcppt::make_unique_ptr<btRigidBody>(
			create_construction_info(
				motion_state_.get(),
				shape_.get(),
				params.mass()))),
	user_data_(
		params.user_data())
{
	body_->setUserPointer(
		this);
	body_->setLinearVelocity(
		structure_cast(
			params.linear_velocity()));
	body_->setAngularVelocity(
		structure_cast(
			params.angular_velocity()));
	body_->setActivationState(
		DISABLE_DEACTIVATION);
}

fruitcut::fruitlib::physics::matrix4 const
fruitcut::fruitlib::physics::rigid_body::object::world_transform() const
{
	btTransform deftr;
	motion_state_->getWorldTransform(
		deftr);
	return 
		fcppt::math::matrix::translation(
			structure_cast<vector3>(
			 	deftr.getOrigin())) *
		matrix3_to_matrix4<scalar>(
			structure_cast<matrix3>(
				deftr.getBasis()));
}

fruitcut::fruitlib::physics::matrix4 const
fruitcut::fruitlib::physics::rigid_body::object::transformation() const
{
	btTransform deftr;
	motion_state_->getWorldTransform(
		deftr);
	return 
		matrix3_to_matrix4<scalar>(
			structure_cast<matrix3>(
				deftr.getBasis()));
}

fruitcut::fruitlib::physics::vector3 const
fruitcut::fruitlib::physics::rigid_body::object::linear_velocity() const
{
	return 
		structure_cast<vector3>(
			body_->getLinearVelocity());
}

fruitcut::fruitlib::physics::vector3 const
fruitcut::fruitlib::physics::rigid_body::object::angular_velocity() const
{
	return 
		structure_cast<vector3>(
			body_->getAngularVelocity());
}

fruitcut::fruitlib::physics::vector3 const
fruitcut::fruitlib::physics::rigid_body::object::position() const
{
	btTransform deftr;
	motion_state_->getWorldTransform(
		deftr);
	return 
		structure_cast<vector3>(
			deftr.getOrigin());
}

fruitcut::fruitlib::physics::rigid_body::user_data const &
fruitcut::fruitlib::physics::rigid_body::object::user_data() const
{
	return user_data_;
}

fruitcut::fruitlib::physics::rigid_body::object::~object()
{
}
