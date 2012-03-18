#include <fruitlib/physics/matrix3.hpp>
#include <fruitlib/physics/matrix3_to_matrix4.hpp>
#include <fruitlib/physics/matrix4.hpp>
#include <fruitlib/physics/matrix4_to_matrix3.hpp>
#include <fruitlib/physics/scalar.hpp>
#include <fruitlib/physics/structure_cast.hpp>
#include <fruitlib/physics/vector3.hpp>
#include <fruitlib/physics/rigid_body/object.hpp>
#include <fruitlib/physics/rigid_body/parameters.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/optional.hpp>
#include <fcppt/math/matrix/arithmetic.hpp>
#include <fcppt/math/matrix/object_impl.hpp>
#include <fcppt/math/matrix/translation.hpp>
#include <fcppt/config/external_begin.hpp>
#include <LinearMath/btDefaultMotionState.h>
#include <LinearMath/btMotionState.h>
#include <LinearMath/btTransform.h>
#include <LinearMath/btVector3.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include <fcppt/config/external_end.hpp>


namespace
{
btRigidBody::btRigidBodyConstructionInfo const
create_construction_info(
	btMotionState *motion_state,
	btCollisionShape *shape,
	fruitlib::physics::rigid_body::optional_mass const &mass)
{
	btVector3 inertia(0,0,0);
	if (mass)
		shape->calculateLocalInertia(
			(*mass).get(),
			inertia);
	return
		btRigidBody::btRigidBodyConstructionInfo(
				mass
				?
					(*mass).get()
				:
					static_cast<btScalar>(0),
				motion_state,
				shape,
				inertia);
}
}

fruitlib::physics::rigid_body::object::object(
	rigid_body::parameters const &params)
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

fruitlib::physics::rigid_body::transformation::value_type const
fruitlib::physics::rigid_body::object::world_transform() const
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

fruitlib::physics::rigid_body::transformation::value_type const
fruitlib::physics::rigid_body::object::transformation() const
{
	btTransform deftr;
	motion_state_->getWorldTransform(
		deftr);
	return
		matrix3_to_matrix4<scalar>(
			structure_cast<matrix3>(
				deftr.getBasis()));
}

fruitlib::physics::rigid_body::linear_velocity::value_type const
fruitlib::physics::rigid_body::object::linear_velocity() const
{
	return
		structure_cast<vector3>(
			body_->getLinearVelocity());
}

fruitlib::physics::rigid_body::angular_velocity::value_type const
fruitlib::physics::rigid_body::object::angular_velocity() const
{
	return
		structure_cast<vector3>(
			body_->getAngularVelocity());
}

fruitlib::physics::rigid_body::position::value_type const
fruitlib::physics::rigid_body::object::position() const
{
	btTransform deftr;
	motion_state_->getWorldTransform(
		deftr);
	return
		structure_cast<vector3>(
			deftr.getOrigin());
}

fruitlib::physics::rigid_body::user_data const &
fruitlib::physics::rigid_body::object::user_data() const
{
	return user_data_;
}

fruitlib::physics::rigid_body::object::~object()
{
}
