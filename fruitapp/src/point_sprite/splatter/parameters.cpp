#include <fruitapp/point_sprite/splatter/parameters.hpp>
#include <sge/texture/part_shared_ptr.hpp>


fruitapp::point_sprite::splatter::parameters::parameters(
	fruitapp::projection_manager::object const &_projection_manager,
	fruitapp::point_sprite::connection &_connection,
	fruitapp::point_sprite::splatter::position const &_position,
	fruitapp::point_sprite::splatter::linear_velocity const &_linear_velocity,
	fruitapp::point_sprite::splatter::acceleration const &_acceleration,
	fruitapp::point_sprite::splatter::size const &_size,
	fruitapp::point_sprite::color const &_color,
	sge::texture::const_optional_part_ref const &_texture,
	fruitapp::ingame_clock::duration const &_life_time,
	fruitapp::ingame_clock const &_clock)
:
	projection_manager_(
		_projection_manager),
	connection_(
		_connection),
	position_(
		_position),
	linear_velocity_(
		_linear_velocity),
	acceleration_(
		_acceleration),
	size_(
		_size),
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

fruitapp::projection_manager::object const &
fruitapp::point_sprite::splatter::parameters::projection_manager() const
{
	return projection_manager_;
}

fruitapp::point_sprite::connection &
fruitapp::point_sprite::splatter::parameters::connection() const
{
	return connection_;
}

fruitapp::point_sprite::splatter::position const &
fruitapp::point_sprite::splatter::parameters::position() const
{
	return position_;
}

fruitapp::point_sprite::splatter::linear_velocity const &
fruitapp::point_sprite::splatter::parameters::linear_velocity() const
{
	return linear_velocity_;
}

fruitapp::point_sprite::splatter::acceleration const &
fruitapp::point_sprite::splatter::parameters::acceleration() const
{
	return acceleration_;
}

fruitapp::point_sprite::splatter::size const &
fruitapp::point_sprite::splatter::parameters::size() const
{
	return size_;
}

fruitapp::point_sprite::color const &
fruitapp::point_sprite::splatter::parameters::color() const
{
	return color_;
}

sge::texture::const_optional_part_ref const &
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
