#include "spawner.hpp"
#include "manager.hpp"
#include "../../physics/scalar.hpp"
#include "../../physics/vector3.hpp"
#include "../../physics/matrix4.hpp"
#include "../../create_rng.hpp"
#include "../../json/find_member.hpp"
#include "../../json/parse_random_inclusive_range.hpp"
#include "../../math/box_radius.hpp"
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
#include <fcppt/math/pi.hpp>
#include <fcppt/math/rad_to_deg.hpp>
#include <fcppt/text.hpp>
#include <fcppt/assert.hpp>
#include <cmath>
#include <iostream>


#include <sge/renderer/vector4.hpp>
#include "../../math/plane/normalize.hpp"
#include "../../math/plane/basic.hpp"

namespace
{
sge::renderer::scalar 
near_plane_distance(
	sge::renderer::matrix4 const &m)
{
	typedef
	fruitcut::math::plane::basic<sge::renderer::scalar,3>
	plane_type;

	sge::renderer::vector4 const 
		fourth_row(
			m[3][0],
			m[3][1],
			m[3][2],
			m[3][3]),
		third_row(
			m[2][0],
			m[2][1],
			m[2][2],
			m[2][3]),
		plane_vec4 = 
			fourth_row + third_row;

	plane_type const near_plane = 
		fruitcut::math::plane::normalize(
			plane_type(
				sge::renderer::vector3(
					plane_vec4[0],
					plane_vec4[1],
					plane_vec4[2]),
				plane_vec4[3]));

	return near_plane.lambda();
}
}

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
		json::parse_random_inclusive_range<physics::scalar>(
 			json::find_member<sge::parse::json::array>(
				_config_file,
				FCPPT_TEXT("fruit-spawner/spawn-range-seconds"))),
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
		json::parse_random_inclusive_range<physics::scalar>(
 			json::find_member<sge::parse::json::array>(
				_config_file,
				FCPPT_TEXT("fruit-spawner/linear-velocity-range"))),
		create_rng()),
	angular_velocity_rng_(
		json::parse_random_inclusive_range<physics::scalar>(
 			json::find_member<sge::parse::json::array>(
				_config_file,
				FCPPT_TEXT("fruit-spawner/angular-velocity-range"))),
		create_rng()),
	angle_rng_(
		fcppt::random::make_inclusive_range(
				0.0f,
				1.0f)),
	timer_(
		sge::time::duration(),
		sge::time::activation_state::inactive,
		_callback),
	spawn_signal_()
{
	reset_timer();
}

fcppt::signal::auto_connection
fruitcut::app::fruit::spawner::spawn_callback(
	spawn_callback_function const &callback)
{
	return 
		spawn_signal_.connect(
			callback);
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
			::near_plane_distance(
				camera_.mvp()),
		height = 
			static_cast<physics::scalar>(
				2 * 
				std::tan(
					perspective.fov()/2) * distance_to_origin),
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

	physics::scalar const x(
		0.25f +
		0.5f *
		x_rng_());

	physics::vector3 const position(
		zero_plane.left() + x * zero_plane.size().w(),
		zero_plane.pos().y() - 
		math::box_radius(
			chosen_prototype.bounding_box()),
		0);

	physics::scalar const magnitude = 
		linear_velocity_rng_();

	float const min_phi = 
		std::min( 2.0f * x, 1.0f ) * 0.25f * fcppt::math::pi<float>();
	float const max_phi =
		- std::min( 2.0f * (1.0f - x), 1.0f ) * 0.25f * fcppt::math::pi<float>();
	physics::scalar const phi =
		static_cast<physics::scalar>(
			angle_rng_() *
			(
				max_phi - 
				min_phi) + 
				min_phi +
				fcppt::math::pi<float>() * 0.5f);

	physics::vector3 const linear_velocity(
		magnitude * std::cos(phi),
		magnitude * std::sin(phi),
		0.f
		);

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

	spawn_signal_();
}

void
fruitcut::app::fruit::spawner::render()
{
}
