#include <fruitapp/point_sprite/splatter/parameters.hpp>
#include <sge/texture/part_ptr.hpp>


fruitapp::point_sprite::splatter::parameters::parameters(
	point_sprite::connection &_connection,
	point_sprite::splatter::position const &_position,
	point_sprite::splatter::linear_velocity const &_linear_velocity,
	point_sprite::splatter::acceleration const &_acceleration,
	point_sprite::splatter::size const &_size,
	point_sprite::color const &_color,
	sge::texture::part_ptr const _texture,
	fruitapp::ingame_clock::duration const &_life_time,
	fruitapp::ingame_clock const &_clock)
:
	connection_(
		_connection),
	position_(
		_position.get()),
	linear_velocity_(
		_linear_velocity.get()),
	acceleration_(
		_acceleration.get()),
	size_(
		_size.get()),
	color_(
		_color),
	texture_(
		_texture),
	life_time_(
		_life_time),
	clock_(
		_clock)
{
}

fruitapp::point_sprite::connection &
fruitapp::point_sprite::splatter::parameters::connection() const
{
	return connection_;
}

fruitapp::point_sprite::splatter::position::value_type const &
fruitapp::point_sprite::splatter::parameters::position() const
{
	return position_;
}

fruitapp::point_sprite::splatter::linear_velocity::value_type const &
fruitapp::point_sprite::splatter::parameters::linear_velocity() const
{
	return linear_velocity_;
}

fruitapp::point_sprite::splatter::acceleration::value_type const &
fruitapp::point_sprite::splatter::parameters::acceleration() const
{
	return acceleration_;
}

fruitapp::point_sprite::splatter::size::value_type const &
fruitapp::point_sprite::splatter::parameters::size() const
{
	return size_;
}

fruitapp::point_sprite::color const &
fruitapp::point_sprite::splatter::parameters::color() const
{
	return color_;
}

sge::texture::part_ptr const
fruitapp::point_sprite::splatter::parameters::texture() const
{
	return texture_;
}

fruitapp::ingame_clock::duration const &
fruitapp::point_sprite::splatter::parameters::life_time() const
{
	return life_time_;
}

fruitapp::ingame_clock const &
fruitapp::point_sprite::splatter::parameters::clock() const
{
	return clock_;
}

fruitapp::point_sprite::splatter::parameters::~parameters()
{
}
