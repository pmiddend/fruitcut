#include "spawner.hpp"
#include "manager.hpp"
#include "../../fruitlib/physics/scalar.hpp"
#include "../../fruitlib/physics/vector3.hpp"
#include "../../fruitlib/physics/matrix4.hpp"
#include "../../fruitlib/json/find_and_convert_member.hpp"
#include "../../fruitlib/json/parse_random_inclusive_range.hpp"
#include "../../fruitlib/math/box_radius.hpp"
#include "../../fruitlib/math/view_plane_rect.hpp"
#include "../../fruitlib/scenic/events/update.hpp"
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
#include "../../fruitlib/math/plane/normalize.hpp"
#include "../../fruitlib/math/plane/basic.hpp"

fruitcut::app::fruit::spawner::spawner(
	fruitlib::scenic::parent const &_parent,
	fruit::manager &_manager,
	fruitlib::random_generator &_random_generator,
	sge::parse::json::object const &_config_file,
	sge::camera::object const &_camera,
	sge::time::callback const &_callback)
:
	node_base(
		_parent),
	manager_(
		_manager),
	camera_(
		_camera),
	seconds_rng_(
		fruitlib::json::parse_random_inclusive_range<fruitlib::physics::scalar>(
 			fruitlib::json::find_and_convert_member<sge::parse::json::array>(
				_config_file,
				fruitlib::json::path(
					FCPPT_TEXT("fruit-spawner"))
					/ FCPPT_TEXT("spawn-range-seconds"))),
		_random_generator),
	prototype_rng_(
		fcppt::random::make_last_exclusive_range(
			static_cast<prototype_sequence::size_type>(
				0),
			manager_.prototypes().size()),
		_random_generator),
	x_rng_(
		fcppt::random::make_inclusive_range(
 			static_cast<fruitlib::physics::scalar>(
				0),
			static_cast<fruitlib::physics::scalar>(
				1)),
		_random_generator),
	linear_velocity_rng_(
		fruitlib::json::parse_random_inclusive_range<fruitlib::physics::scalar>(
 			fruitlib::json::find_and_convert_member<sge::parse::json::array>(
				_config_file,
				fruitlib::json::path(
					FCPPT_TEXT("fruit-spawner"))
					/ FCPPT_TEXT("linear-velocity-range"))),
		_random_generator),
	angular_velocity_rng_(
		fruitlib::json::parse_random_inclusive_range<fruitlib::physics::scalar>(
 			fruitlib::json::find_and_convert_member<sge::parse::json::array>(
				_config_file,
				fruitlib::json::path(
					FCPPT_TEXT("fruit-spawner"))
					/ FCPPT_TEXT("angular-velocity-range"))),
		_random_generator),
	angle_rng_(
		fcppt::random::make_inclusive_range(
				0.0f,
				1.0f),
		_random_generator),
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
fruitcut::app::fruit::spawner::react(
	fruitlib::scenic::events::update const &)
{
	if(camera_.projection_object().empty())
		return;

	if(!timer_.active() || !timer_.expired())
		return;

	reset_timer();

	typedef
	fcppt::math::box::basic<sge::renderer::scalar,2>
	scalar_rect;

	// zero plane because it's the visible plane located at z = 0
	scalar_rect const zero_plane(
		fruitlib::math::view_plane_rect(
			camera_.mvp(),
			camera_.projection_object().get<sge::camera::projection::perspective>()));

	prototype_sequence::size_type const prototype_index =
		prototype_rng_();

	FCPPT_ASSERT(
		prototype_index < manager_.prototypes().size());

	prototype const &chosen_prototype = 
		manager_.prototypes()[prototype_index];

	fruitlib::physics::scalar const x(
		0.25f +
		0.5f *
		x_rng_());

	sge::renderer::scalar const bounding_box_radius = 
		fruitlib::math::box_radius(
			chosen_prototype.bounding_box());

	fruitlib::physics::vector3 const position(
		zero_plane.left() + x * zero_plane.size().w(),
		zero_plane.pos().y() - bounding_box_radius,
		-bounding_box_radius);

	fruitlib::physics::scalar const magnitude = 
		linear_velocity_rng_();

	float const min_phi = 
		std::min( 2.0f * x, 1.0f ) * 0.25f * fcppt::math::pi<float>();
	float const max_phi =
		- std::min( 2.0f * (1.0f - x), 1.0f ) * 0.25f * fcppt::math::pi<float>();
	fruitlib::physics::scalar const phi =
		static_cast<fruitlib::physics::scalar>(
			angle_rng_() *
			(
				max_phi - 
				min_phi) + 
				min_phi +
				fcppt::math::pi<float>() * 0.5f);

	fruitlib::physics::vector3 const linear_velocity(
		magnitude * std::cos(phi),
		magnitude * std::sin(phi),
		0.f
		);

	// Could be generated as well, not for now though
	fruitlib::physics::vector3 const angular_velocity(
		angular_velocity_rng_(),
		angular_velocity_rng_(),
		angular_velocity_rng_());

	fruitlib::physics::scalar const mass = 
		chosen_prototype.mass();

	manager_.spawn(
		chosen_prototype,
		mass,
		position,
		linear_velocity,
		angular_velocity);

	spawn_signal_();
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
