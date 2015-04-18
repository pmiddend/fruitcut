#include <fruitapp/point_sprite/splatter/object.hpp>
#include <fruitapp/point_sprite/splatter/parameters.hpp>
#include <fruitapp/projection_manager/object.hpp>
#include <sge/renderer/vector2.hpp>
#include <sge/renderer/vector4.hpp>
#include <sge/renderer/target/base.hpp>
#include <sge/sprite/intrusive/connection.hpp>
#include <sge/sprite/roles/center.hpp>
#include <sge/sprite/roles/color.hpp>
#include <sge/sprite/roles/connection.hpp>
#include <sge/sprite/roles/size.hpp>
#include <sge/sprite/roles/texture0.hpp>
#include <sge/timer/elapsed_fractional_and_reset.hpp>
#include <sge/timer/parameters.hpp>
#include <fcppt/make_ref.hpp>
#include <fcppt/math/matrix/arithmetic.hpp>
#include <fcppt/math/matrix/vector.hpp>
#include <fcppt/math/vector/arithmetic.hpp>
#include <fcppt/math/vector/output.hpp>
#include <fcppt/config/external_begin.hpp>
#include <chrono>
#include <fcppt/config/external_end.hpp>


fruitapp::point_sprite::splatter::object::object(
	fruitapp::point_sprite::splatter::parameters const &p)
:
	projection_manager_(
		p.projection_manager()),
	position_(
		p.position()),
	linear_velocity_(
		p.linear_velocity()),
	acceleration_(
		p.acceleration()),
	object_(
		sge::sprite::roles::center{} =
			projection_manager_.project_point(
				position_.get()
			),
		sge::sprite::roles::size{} =
			fruitapp::point_sprite::object::dim(
				p.size().get(),
				p.size().get()
			),
		sge::sprite::roles::texture0{} =
			p.texture(),
		sge::sprite::roles::connection{} =
			fcppt::make_ref(
				p.connection()
			),
		sge::sprite::roles::color{} =
			p.color()
	),
	life_timer_(
		fruitapp::ingame_timer::parameters(
			p.clock(),
			p.life_time())),
	second_timer_(
		fruitapp::ingame_timer::parameters(
			p.clock(),
			std::chrono::seconds(
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
	object_.center(
		projection_manager_.project_point(
			position_.get()));
	linear_velocity_ +=
		fruitapp::point_sprite::splatter::linear_velocity(
			time_delta * acceleration_.get());
}

bool
fruitapp::point_sprite::splatter::object::dead() const
{
	return life_timer_.expired();
}
