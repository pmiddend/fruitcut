#include "spawner.hpp"
#include "manager.hpp"
#include "../../physics/scalar.hpp"
#include "../../physics/vector3.hpp"
#include "../../physics/matrix4.hpp"
#include "../create_rng.hpp"
#include "../../json/find_member.hpp"
#include <sge/time/funit.hpp>
#include <sge/time/duration.hpp>
#include <sge/time/second_f.hpp>
#include <sge/time/activation_state.hpp>
#include <sge/camera/object.hpp>
#include <sge/camera/projection/object.hpp>
#include <sge/camera/projection/perspective.hpp>
#include <fcppt/random/make_inclusive_range.hpp>
#include <fcppt/random/make_last_exclusive_range.hpp>
#include <fcppt/math/box/basic_impl.hpp>
#include <fcppt/math/box/output.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/math/vector/length.hpp>
#include <fcppt/math/vector/output.hpp>
#include <fcppt/text.hpp>
#include <fcppt/assert.hpp>
#include <cmath>
#include <iostream>

fruitcut::app::fruit::spawner::spawner(
	manager &_manager,
	sge::parse::json::object const &_config_file,
	sge::camera::object const &_camera,
	sge::time::callback const &_callback)
:
	manager_(
		_manager),
	camera_(
		_camera),
	seconds_rng_(
		fcppt::random::make_inclusive_range(
 			json::find_member<sge::time::funit>(
				_config_file,
				FCPPT_TEXT("fruit-spawner/min-seconds")),
			json::find_member<sge::time::funit>(
				_config_file,
				FCPPT_TEXT("fruit-spawner/max-seconds"))),
		create_rng()),
	prototype_rng_(
		fcppt::random::make_last_exclusive_range(
			static_cast<prototype_sequence::size_type>(
				0),
			manager_.prototypes().size()),
		create_rng()),
	x_rng_(
		fcppt::random::make_inclusive_range(
 			static_cast<physics::scalar>(
				0),
			static_cast<physics::scalar>(
				1))),
	linear_velocity_rng_(
		fcppt::random::make_inclusive_range(
 			json::find_member<physics::scalar>(
				_config_file,
				FCPPT_TEXT("fruit-spawner/min-linear-velocity")),
			json::find_member<physics::scalar>(
				_config_file,
				FCPPT_TEXT("fruit-spawner/max-linear-velocity"))),
		create_rng()),
	angular_velocity_rng_(
		fcppt::random::make_inclusive_range(
 			json::find_member<physics::scalar>(
				_config_file,
				FCPPT_TEXT("fruit-spawner/min-angular-velocity")),
			json::find_member<physics::scalar>(
				_config_file,
				FCPPT_TEXT("fruit-spawner/max-angular-velocity"))),
		create_rng()),
	timer_(
		sge::time::duration(),
		sge::time::activation_state::inactive,
		_callback)
{
	reset_timer();
}

void
fruitcut::app::fruit::spawner::update()
{
	if(!timer_.active() || !timer_.expired())
		return;

	reset_timer();

	typedef
	fcppt::math::box::basic<physics::scalar,2>
	scalar_rect;

	sge::camera::projection::perspective const &perspective = 
		camera_.projection_object().get<sge::camera::projection::perspective>();

	physics::scalar const 
		distance_to_origin = 
			static_cast<physics::scalar>(
				fcppt::math::vector::length(
					camera_.gizmo().position())),
		height = 
			static_cast<physics::scalar>(
				std::tan(
					perspective.fov()) * distance_to_origin),
		width = 
			static_cast<physics::scalar>(
				height * perspective.aspect());

	// zero plane because it's the visible plane located at z = 0
	scalar_rect const zero_plane(
		scalar_rect::vector(
			-width/2,
			-height/2),
		scalar_rect::dim(
			width,
			height));

	prototype_sequence::size_type const prototype_index =
		prototype_rng_();

	FCPPT_ASSERT(
		prototype_index < manager_.prototypes().size());

	prototype const &chosen_prototype = 
		manager_.prototypes()[prototype_index];

	physics::vector3 const position(
		zero_plane.left() + x_rng_() * zero_plane.size().w(),
		zero_plane.pos().y(),
		0);

	physics::scalar const 
		flatness = 
			// 0.5 somehow means that we stay inside [left,right] for the x
			// coordinate. Higher values mean more flat.
			static_cast<physics::scalar>(
				0.5),
		magnitude = 
			linear_velocity_rng_();

	physics::vector3 const linear_velocity(
		-flatness * (position.x() - zero_plane.left()) + flatness * (zero_plane.right() - position.x()),
		magnitude,
		0);

	// Could be generated as well, not for now though
	physics::vector3 const angular_velocity(
		angular_velocity_rng_(),
		angular_velocity_rng_(),
		angular_velocity_rng_());

	// TODO: Read into prototype
	physics::scalar const mass = 
		100;

	manager_.spawn(
		chosen_prototype,
		mass,
		position,
		linear_velocity,
		angular_velocity);
}

void
fruitcut::app::fruit::spawner::reset_timer()
{
	if(!timer_.active())
		timer_.activate();
	timer_.interval(
		sge::time::second_f(
			seconds_rng_()));
}
