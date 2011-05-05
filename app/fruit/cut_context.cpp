#include "cut_context.hpp"

fruitcut::app::fruit::cut_context::cut_context(
	fruit::object const &_old,
	new_fruit_array const &_new,
	fruit::area const _area)
:
	old_(
		&_old),
	new_(
		_new),
	area_(
		_area)
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
