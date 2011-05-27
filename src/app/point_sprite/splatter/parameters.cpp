#include "parameters.hpp"

fruitcut::app::point_sprite::splatter::parameters::parameters(
	fruitcut::app::point_sprite::system &_system,
	fruitcut::app::point_sprite::splatter::position const &_position,
	fruitcut::app::point_sprite::splatter::linear_velocity const &_linear_velocity,
	fruitcut::app::point_sprite::splatter::acceleration const &_acceleration,
	fruitcut::app::point_sprite::splatter::size const &_size,
	fruitcut::app::point_sprite::color const &_color,
	sge::texture::part_ptr const _texture,
	sge::time::duration const &_life_time,
	sge::time::callback const &_time_callback)
:
	system_(
		_system),
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
	time_callback_(
		_time_callback)
{
}

fruitcut::app::point_sprite::system &
fruitcut::app::point_sprite::splatter::parameters::system() const
{
	return system_;
}

fruitcut::app::point_sprite::splatter::position::value_type const &
fruitcut::app::point_sprite::splatter::parameters::position() const
{
	return position_;
}

fruitcut::app::point_sprite::splatter::linear_velocity::value_type const &
fruitcut::app::point_sprite::splatter::parameters::linear_velocity() const
{
	return linear_velocity_;
}

fruitcut::app::point_sprite::splatter::acceleration::value_type const &
fruitcut::app::point_sprite::splatter::parameters::acceleration() const
{
	return acceleration_;
}

fruitcut::app::point_sprite::splatter::size::value_type const &
fruitcut::app::point_sprite::splatter::parameters::size() const
{
	return size_;
}

fruitcut::app::point_sprite::color const &
fruitcut::app::point_sprite::splatter::parameters::color() const
{
	return color_;
}

sge::texture::part_ptr const
fruitcut::app::point_sprite::splatter::parameters::texture() const
{
	return texture_;
}

sge::time::duration const &
fruitcut::app::point_sprite::splatter::parameters::life_time() const
{
	return life_time_;
}

sge::time::callback const &
fruitcut::app::point_sprite::splatter::parameters::time_callback() const
{
	return time_callback_;
}

fruitcut::app::point_sprite::splatter::parameters::~parameters()
{
}
