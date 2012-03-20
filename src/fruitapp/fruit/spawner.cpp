#include <fruitapp/fruit/manager.hpp>
#include <fruitapp/fruit/spawner.hpp>
#include <fruitlib/math/box_radius.hpp>
#include <fruitlib/math/view_plane_rect.hpp>
#include <fruitlib/math/plane/basic.hpp>
#include <fruitlib/math/plane/normalize.hpp>
#include <fruitlib/physics/matrix4.hpp>
#include <fruitlib/physics/scalar.hpp>
#include <fruitlib/physics/vector3.hpp>
#include <sge/camera/first_person/object.hpp>
#include <sge/camera/projection/invalid.hpp>
#include <sge/camera/projection/perspective.hpp>
#include <sge/parse/json/array.hpp>
#include <sge/parse/json/convert_from.hpp>
#include <sge/parse/json/find_and_convert_member.hpp>
#include <sge/parse/json/object.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/renderer/vector4.hpp>
#include <fcppt/text.hpp>
#include <fcppt/chrono/duration.hpp>
#include <fcppt/math/pi.hpp>
#include <fcppt/math/rad_to_deg.hpp>
#include <fcppt/math/box/object_impl.hpp>
#include <fcppt/math/box/output.hpp>
#include <fcppt/math/vector/object_impl.hpp>
#include <fcppt/math/vector/length.hpp>
#include <fcppt/math/vector/output.hpp>
#include <fcppt/signal/auto_connection.hpp>
#include <fcppt/config/external_begin.hpp>
#include <cmath>
#include <iostream>
#include <fcppt/config/external_end.hpp>


fruitapp::fruit::spawner::spawner(
	fruitlib::scenic::optional_parent const &_parent,
	fruit::manager &_manager,
	fruitlib::random_generator &_random_generator,
	sge::parse::json::object const &_config_file,
	sge::camera::first_person::object const &_camera,
	fruitapp::ingame_clock const &_clock)
:
	node_base(
		_parent),
	manager_(
		_manager),
	camera_(
		_camera),
	seconds_rng_(
		_random_generator,
		uniform_float_variate::distribution(
			uniform_float_variate::distribution::min(
				sge::parse::json::convert_from<uniform_float_variate::distribution::float_type>(
					sge::parse::json::find_and_convert_member<sge::parse::json::array>(
						_config_file,
						sge::parse::json::path(
							FCPPT_TEXT("fruit-spawner"))
							/ FCPPT_TEXT("spawn-range-seconds")).elements[0])),
			uniform_float_variate::distribution::sup(
				sge::parse::json::convert_from<uniform_float_variate::distribution::float_type>(
					sge::parse::json::find_and_convert_member<sge::parse::json::array>(
						_config_file,
						sge::parse::json::path(
							FCPPT_TEXT("fruit-spawner"))
							/ FCPPT_TEXT("spawn-range-seconds")).elements[1])))),
	prototype_rng_(
		_random_generator,
		uniform_prototype_size_variate::distribution(
			uniform_prototype_size_variate::distribution::min(
				0u),
			uniform_prototype_size_variate::distribution::max(
				manager_.prototypes().size()-1))),
	x_rng_(
		_random_generator,
		uniform_physics_variate::distribution(
			uniform_physics_variate::distribution::min(
				0.0f),
			uniform_physics_variate::distribution::sup(
				1.0f))),
	linear_velocity_rng_(
		_random_generator,
		uniform_physics_variate::distribution(
			uniform_physics_variate::distribution::min(
				sge::parse::json::convert_from<uniform_physics_variate::distribution::float_type>(
					sge::parse::json::find_and_convert_member<sge::parse::json::array>(
						_config_file,
						sge::parse::json::path(
							FCPPT_TEXT("fruit-spawner"))
							/ FCPPT_TEXT("linear-velocity-range")).elements[0])),
			uniform_physics_variate::distribution::sup(
				sge::parse::json::convert_from<uniform_physics_variate::distribution::float_type>(
					sge::parse::json::find_and_convert_member<sge::parse::json::array>(
						_config_file,
						sge::parse::json::path(
							FCPPT_TEXT("fruit-spawner"))
							/ FCPPT_TEXT("linear-velocity-range")).elements[1])))),
	angular_velocity_rng_(
		_random_generator,
		uniform_physics_variate::distribution(
			uniform_physics_variate::distribution::min(
				sge::parse::json::convert_from<uniform_physics_variate::distribution::float_type>(
					sge::parse::json::find_and_convert_member<sge::parse::json::array>(
						_config_file,
						sge::parse::json::path(
							FCPPT_TEXT("fruit-spawner"))
							/ FCPPT_TEXT("angular-velocity-range")).elements[0])),
			uniform_physics_variate::distribution::sup(
				sge::parse::json::convert_from<uniform_physics_variate::distribution::float_type>(
					sge::parse::json::find_and_convert_member<sge::parse::json::array>(
						_config_file,
						sge::parse::json::path(
							FCPPT_TEXT("fruit-spawner"))
							/ FCPPT_TEXT("angular-velocity-range")).elements[1])))),
	angle_rng_(
		_random_generator,
		uniform_physics_variate::distribution(
			uniform_physics_variate::distribution::min(
				0.0f),
			uniform_physics_variate::distribution::sup(
				1.0f))),
	timer_(
		fruitapp::ingame_timer::parameters(
			_clock,
			fruitapp::ingame_clock::duration())
			.active(
				false)),
	spawn_signal_()
{
	reset_timer();
}

fcppt::signal::auto_connection
fruitapp::fruit::spawner::spawn_callback(
	spawn_callback_function const &callback)
{
	return
		spawn_signal_.connect(
			callback);
}

void
fruitapp::fruit::spawner::react(
	fruitlib::scenic::events::update const &)
{
	if(fcppt::variant::holds_type<sge::camera::projection::invalid>(camera_.projection_object()))
		return;

	if(!timer_.active() || !timer_.expired())
		return;

	reset_timer();

	typedef
	fcppt::math::box::object<sge::renderer::scalar,2>
	scalar_rect;

	// zero plane because it's the visible plane located at z = 0
	scalar_rect const zero_plane(
		fruitlib::math::view_plane_rect(
			camera_.mvp(),
			camera_.projection_object().get<sge::camera::projection::perspective>()));

	prototype_sequence::size_type const prototype_index =
		prototype_rng_();

	FCPPT_ASSERT_ERROR(
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
fruitapp::fruit::spawner::reset_timer()
{
	if(!timer_.active())
		timer_.active(
			true);
	timer_.interval(
		fcppt::chrono::duration<fruitapp::ingame_clock::float_type>(
			seconds_rng_()));
}
