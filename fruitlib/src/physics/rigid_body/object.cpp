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
#include <fcppt/optional/maybe.hpp>
#include <fcppt/optional/maybe_void.hpp>
#include <fcppt/optional/object.hpp>
#include <fcppt/math/matrix/arithmetic.hpp>
#include <fcppt/math/matrix/object_impl.hpp>
#include <fcppt/math/matrix/translation.hpp>
#include <fcppt/config/external_begin.hpp>
#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <LinearMath/btDefaultMotionState.h>
#include <LinearMath/btMotionState.h>
#include <LinearMath/btTransform.h>
#include <LinearMath/btVector3.h>
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
	fcppt::optional::maybe_void(
		mass,
		[
			shape,
			&inertia
		](
			fruitlib::physics::rigid_body::mass const _mass
		)
		{
			shape->calculateLocalInertia(
				_mass.get(),
				inertia);
		}
	);

	return
		btRigidBody::btRigidBodyConstructionInfo(
			fcppt::optional::maybe(
				mass,
				[]{
					return
						static_cast<btScalar>(0);
				},
				[](
					fruitlib::physics::rigid_body::mass const _mass
				)
				{
					return
						_mass.get();
				}
			),
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
					matrix4_to_matrix3(
						params.transformation())),
				structure_cast(
					params.position())))),
	body_(
		fcppt::make_unique_ptr<btRigidBody>(
			create_construction_info(
				motion_state_.get_pointer(),
				shape_.get_pointer(),
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

fruitlib::physics::rigid_body::transformation const
fruitlib::physics::rigid_body::object::world_transform() const
{
	btTransform deftr;
	motion_state_->getWorldTransform(
		deftr);
	return
		fruitlib::physics::rigid_body::transformation(
			fcppt::math::matrix::translation(
				structure_cast<vector3>(
					deftr.getOrigin())) *
			matrix3_to_matrix4<scalar>(
				structure_cast<matrix3>(
					deftr.getBasis())));
}

fruitlib::physics::rigid_body::transformation const
fruitlib::physics::rigid_body::object::transformation() const
{
	btTransform deftr;
	motion_state_->getWorldTransform(
		deftr);
	return
		fruitlib::physics::rigid_body::transformation(
			matrix3_to_matrix4<scalar>(
				structure_cast<matrix3>(
					deftr.getBasis())));
}

fruitlib::physics::rigid_body::linear_velocity const
fruitlib::physics::rigid_body::object::linear_velocity() const
{
	return
		fruitlib::physics::rigid_body::linear_velocity(
			structure_cast<vector3>(
				body_->getLinearVelocity()));
}

fruitlib::physics::rigid_body::angular_velocity const
fruitlib::physics::rigid_body::object::angular_velocity() const
{
	return
		fruitlib::physics::rigid_body::angular_velocity(
			structure_cast<vector3>(
				body_->getAngularVelocity()));
}

fruitlib::physics::rigid_body::position const
fruitlib::physics::rigid_body::object::position() const
{
	btTransform deftr;
	motion_state_->getWorldTransform(
		deftr);
	return
		fruitlib::physics::rigid_body::position(
			structure_cast<vector3>(
				deftr.getOrigin()));
}

fruitlib::physics::rigid_body::user_data const &
fruitlib::physics::rigid_body::object::user_data() const
{
	return user_data_;
}

fruitlib::physics::rigid_body::object::~object()
{
}
