#include <fruitapp/fruit/cut_context.hpp>
#include <fcppt/move.hpp>

fruitapp::fruit::cut_context::cut_context(
	fruit::object const &_old,
	new_fruit_array const &_new,
	fruit::area const &_area,
	fruitlib::physics::vector3 const &_cut_direction,
	fruit::mesh_unique_ptr _cross_section)
:
	old_(
		_old),
	new_(
		_new),
	area_(
		_area.get()),
	cut_direction_(
		_cut_direction),
	cross_section_(
		fcppt::move(
			_cross_section))
{
}

fruitapp::fruit::object const &
fruitapp::fruit::cut_context::old() const
{
	return old_;
}

fruitapp::fruit::cut_context::new_fruit_array const &
fruitapp::fruit::cut_context::new_fruits() const
{
	return new_;
}

fruitapp::fruit::area::value_type
fruitapp::fruit::cut_context::area() const
{
	return area_;
}

fruitlib::physics::vector3 const &
fruitapp::fruit::cut_context::cut_direction() const
{
	return cut_direction_;
}

fruitapp::fruit::mesh const &
fruitapp::fruit::cut_context::cross_section() const
{
	return
		*cross_section_;
}

fruitapp::fruit::cut_context::~cut_context()
{
}
