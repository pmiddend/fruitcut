#include <fruitapp/point_sprite/parameters.hpp>
#include <sge/renderer/vector2.hpp>
#include <sge/camera/base.hpp>
#include <sge/renderer/target/base.hpp>
#include <sge/renderer/vector4.hpp>
#include <fcppt/math/vector/output.hpp>
#include <fcppt/math/matrix/arithmetic.hpp>
#include <fcppt/math/matrix/vector.hpp>
#include <sge/camera/coordinate_system/object.hpp>
#include <sge/camera/matrix_conversion/world_projection.hpp>
#include <fruitapp/point_sprite/splatter/object.hpp>
#include <fruitapp/point_sprite/splatter/parameters.hpp>
#include <sge/sprite/intrusive/connection.hpp>
#include <sge/sprite/center.hpp>
#include <sge/timer/elapsed_fractional_and_reset.hpp>
#include <sge/timer/parameters.hpp>
#include <fcppt/math/vector/arithmetic.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/chrono/duration.hpp>
#include <fcppt/config/external_end.hpp>


fruitapp::point_sprite::splatter::object::object(
	fruitapp::point_sprite::splatter::parameters const &p)
:
	camera_(
		p.camera()),
	target_(
		p.target()),
	position_(
		p.position()),
	linear_velocity_(
		p.linear_velocity()),
	acceleration_(
		p.acceleration()),
	object_(
		fruitapp::point_sprite::parameters()
			.center(
				this->determine_center())
			.size(
				fruitapp::point_sprite::object::dim(
					p.size().get(),
					p.size().get()))
			.texture(
				p.texture())
			.connection(
				p.connection())
			.color(
				p.color())),
	life_timer_(
		fruitapp::ingame_timer::parameters(
			p.clock(),
			p.life_time())),
	second_timer_(
		fruitapp::ingame_timer::parameters(
			p.clock(),
			boost::chrono::seconds(
				1)))
{

}

fruitapp::point_sprite::splatter::object::~object()
{
}

void
fruitapp::point_sprite::splatter::object::update()
{
	fruitapp::point_sprite::object::unit const time_delta =
		sge::timer::elapsed_fractional_and_reset<fruitapp::point_sprite::object::unit>(
			second_timer_);

	position_ =
		fruitapp::point_sprite::splatter::position(
			position_.get() +
			time_delta *
			linear_velocity_.get());
	sge::sprite::center(
		object_,
		this->determine_center());
	linear_velocity_ +=
		fruitapp::point_sprite::splatter::linear_velocity(
			time_delta * acceleration_.get());
}

bool
fruitapp::point_sprite::splatter::object::dead() const
{
	return life_timer_.expired();
}

fruitapp::point_sprite::object::vector const
fruitapp::point_sprite::splatter::object::determine_center() const
{
	sge::renderer::pixel_rect const viewport_rect(
		target_.viewport().get());

	sge::renderer::vector4 const result =
		sge::camera::matrix_conversion::world_projection(
			camera_.coordinate_system(),
			camera_.projection_matrix()) *
		sge::renderer::vector4(
			position_.get().x(),
			position_.get().y(),
			position_.get().z(),
			1.0f);

	sge::renderer::vector2 const
		result_2d(
			result.x() / result.w(),
			result.y() / result.w()),
		result_2d_noninverted(
			(result_2d + sge::renderer::vector2(1.0f,1.0f)) /
			sge::renderer::vector2(2.0f,2.0f));

	return
		sge::renderer::vector2(
			result_2d_noninverted.x(),
			1.0f - result_2d_noninverted.y()) *
		sge::renderer::vector2(
			static_cast<sge::renderer::scalar>(
				viewport_rect.w()),
			static_cast<sge::renderer::scalar>(
				viewport_rect.h()));
}
