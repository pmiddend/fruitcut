#include <fruitapp/fruit/manager.hpp>
#include <fruitapp/fruit/spawner.hpp>
#include <fruitapp/projection_manager/object.hpp>
#include <fruitapp/projection_manager/projection_change_callback.hpp>
#include <fruitlib/math/box_radius.hpp>
#include <fruitlib/math/view_plane_rect.hpp>
#include <fruitlib/math/plane/basic.hpp>
#include <fruitlib/math/plane/normalize.hpp>
#include <fruitlib/physics/matrix4.hpp>
#include <fruitlib/physics/scalar.hpp>
#include <fruitlib/physics/vector3.hpp>
#include <sge/camera/base.hpp>
#include <sge/camera/coordinate_system/object.hpp>
#include <sge/camera/matrix_conversion/world_projection.hpp>
#include <sge/parse/json/array.hpp>
#include <sge/parse/json/convert_from.hpp>
#include <sge/parse/json/find_and_convert_member.hpp>
#include <sge/parse/json/object.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/renderer/vector4.hpp>
#include <fcppt/text.hpp>
#include <fcppt/math/pi.hpp>
#include <fcppt/math/rad_to_deg.hpp>
#include <fcppt/math/box/object_impl.hpp>
#include <fcppt/math/box/output.hpp>
#include <fcppt/math/vector/length.hpp>
#include <fcppt/math/vector/object_impl.hpp>
#include <fcppt/math/vector/output.hpp>
#include <fcppt/signal/auto_connection.hpp>
#include <fcppt/config/external_begin.hpp>
#include <algorithm>
#include <chrono>
#include <cmath>
#include <functional>
#include <fcppt/config/external_end.hpp>


fruitapp::fruit::spawner::spawner(
	fruitlib::scenic::optional_parent const &_parent,
	fruit::manager &_manager,
	fruitlib::random_generator &_random_generator,
	sge::parse::json::object const &_config_file,
	sge::camera::base const &_camera,
	fruitapp::ingame_clock const &_clock,
	fruitapp::projection_manager::object &_projection_manager)
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
			uniform_float_variate::distribution::param_type::min(
				sge::parse::json::convert_from<uniform_float_variate::distribution::result_type>(
					sge::parse::json::find_and_convert_member<sge::parse::json::array>(
						_config_file,
						sge::parse::json::path(
							FCPPT_TEXT("spawn-range-seconds"))).elements[0])),
			uniform_float_variate::distribution::param_type::sup(
				sge::parse::json::convert_from<uniform_float_variate::distribution::result_type>(
					sge::parse::json::find_and_convert_member<sge::parse::json::array>(
						_config_file,
						sge::parse::json::path(
							FCPPT_TEXT("spawn-range-seconds"))).elements[1])))),
	random_prototype_(
		_random_generator,
		manager_.prototypes()),
	x_rng_(
		_random_generator,
		uniform_physics_variate::distribution(
			uniform_physics_variate::distribution::param_type::min(
				0.0f),
			uniform_physics_variate::distribution::param_type::sup(
				1.0f))),
	linear_velocity_rng_(
		_random_generator,
		uniform_physics_variate::distribution(
			uniform_physics_variate::distribution::param_type::min(
				sge::parse::json::convert_from<uniform_physics_variate::distribution::result_type>(
					sge::parse::json::find_and_convert_member<sge::parse::json::array>(
						_config_file,
						sge::parse::json::path(
							FCPPT_TEXT("linear-velocity-range"))).elements[0])),
			uniform_physics_variate::distribution::param_type::sup(
				sge::parse::json::convert_from<uniform_physics_variate::distribution::result_type>(
					sge::parse::json::find_and_convert_member<sge::parse::json::array>(
						_config_file,
						sge::parse::json::path(
							FCPPT_TEXT("linear-velocity-range"))).elements[1])))),
	angular_velocity_rng_(
		_random_generator,
		uniform_physics_variate::distribution(
			uniform_physics_variate::distribution::param_type::min(
				sge::parse::json::convert_from<uniform_physics_variate::distribution::result_type>(
					sge::parse::json::find_and_convert_member<sge::parse::json::array>(
						_config_file,
						sge::parse::json::path(
							FCPPT_TEXT("angular-velocity-range"))).elements[0])),
			uniform_physics_variate::distribution::param_type::sup(
				sge::parse::json::convert_from<uniform_physics_variate::distribution::result_type>(
					sge::parse::json::find_and_convert_member<sge::parse::json::array>(
						_config_file,
						sge::parse::json::path(
							FCPPT_TEXT("angular-velocity-range"))).elements[1])))),
	angle_rng_(
		_random_generator,
		uniform_physics_variate::distribution(
			uniform_physics_variate::distribution::param_type::min(
				0.0f),
			uniform_physics_variate::distribution::param_type::sup(
				1.0f))),
	timer_(
		fruitapp::ingame_timer::parameters(
			_clock,
			fruitapp::ingame_clock::duration())
			.active(
				false)),
	spawn_signal_(),
	perspective_projection_information_(),
	projection_change_connection_(
		_projection_manager.projection_change_callback(
			fruitapp::projection_manager::projection_change_callback{
				std::bind(
					&spawner::projection_change,
					this,
					std::placeholders::_1
				)
			},
			fruitapp::projection_manager::trigger_early(
				true
			)
		)
	)
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
	if(!perspective_projection_information_.has_value())
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
			sge::camera::matrix_conversion::world_projection(
				camera_.coordinate_system(),
				camera_.projection_matrix()),
			perspective_projection_information_.get_unsafe())); // TODO

	fruitapp::fruit::prototype const &chosen_prototype =
		*random_prototype_.value();

	fruitlib::physics::scalar const x(
		0.25f +
		0.5f *
		x_rng_());

	sge::renderer::scalar const bounding_box_radius =
		fruitlib::math::box_radius(
			chosen_prototype.bounding_box());

	fruitlib::physics::rigid_body::position const position(
		fruitlib::physics::vector3(
			zero_plane.left() + x * zero_plane.size().w(),
			zero_plane.pos().y() - bounding_box_radius,
			-bounding_box_radius));

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

	fruitlib::physics::rigid_body::linear_velocity const linear_velocity(
		fruitlib::physics::vector3(
			magnitude * std::cos(phi),
			magnitude * std::sin(phi),
			0.f));

	// Could be generated as well, not for now though
	fruitlib::physics::rigid_body::angular_velocity const angular_velocity(
		fruitlib::physics::vector3(
			angular_velocity_rng_(),
			angular_velocity_rng_(),
			angular_velocity_rng_()));

	fruitlib::physics::rigid_body::mass const mass(
		chosen_prototype.mass());

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
		std::chrono::duration<fruitapp::ingame_clock::float_type>(
			seconds_rng_()));
}

void
fruitapp::fruit::spawner::projection_change(
	fruitlib::perspective_projection_information const &_perspective_projection_information)
{
	perspective_projection_information_ =
		fruitlib::optional_perspective_projection_information(
			_perspective_projection_information);
}
