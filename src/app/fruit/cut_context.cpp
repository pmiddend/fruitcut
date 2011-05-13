#include "cut_context.hpp"

fruitcut::app::fruit::cut_context::cut_context(
	fruit::object const &_old,
	new_fruit_array const &_new,
	fruit::area const _area,
	physics::vector3 const &_cut_direction)
:
	old_(
		&_old),
	new_(
		_new),
	area_(
		_area),
	cut_direction_(
		_cut_direction)
{
}

fruitcut::app::fruit::object const &
fruitcut::app::fruit::cut_context::old() const
{
	return *old_;
}

fruitcut::app::fruit::cut_context::new_fruit_array const &
fruitcut::app::fruit::cut_context::new_fruits() const
{
	return new_;
}

fruitcut::app::fruit::area
fruitcut::app::fruit::cut_context::area() const
{
	return area_;
}

fruitcut::physics::vector3 const &
fruitcut::app::fruit::cut_context::cut_direction() const
{
	return cut_direction_;
}
