#include "spawner.hpp"
#include "../../json/find_member.hpp"
#include <sge/time/funit.hpp>
#include <sge/time/duration.hpp>
#include <sge/time/second_f.hpp>
#include <sge/time/activation_state.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/camera/object.hpp>
#include <sge/camera/projection/object.hpp>
#include <sge/camera/projection/perspective.hpp>
#include <fcppt/random/make_inclusive_range.hpp>
#include <fcppt/math/box/basic_impl.hpp>
#include <fcppt/math/vector/length.hpp>
#include <fcppt/text.hpp>
#include <cmath>

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
				FCPPT_TEXT("fruit-spawner/max-seconds")))),
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
	if(!timer_.active() || !timer_.update_b())
		return;

	typedef
	fcppt::math::box::basic<sge::renderer::scalar,2>
	scalar_rect;

	sge::camera::projection::perspective const &perspective = 
		camera_.projection_object().get<sge::camera::projection::perspective>();

	sge::renderer::scalar const 
		distance_to_origin = 
			fcppt::math::vector::length(
				camera_.gizmo().position()),
		height = 
			2 * std::tan(
				perspective.fov()) * distance_to_origin,
		width = 
			height * perspective.aspect();

	// zero plane because it's the visible plane located at z = 0
	scalar_rect const zero_plane(
		scalar_rect::vector(
			-width/2,
			-height/2),
		scalar_rect::dim(
			width,
			height));

	
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
