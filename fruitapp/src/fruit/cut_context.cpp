#include <fruitapp/fruit/cut_context.hpp>
#include <fcppt/move.hpp>

fruitapp::fruit::cut_context::cut_context(
	fruitapp::fruit::object const &_old,
	new_fruit_array const &_new,
	fruitapp::fruit::area const &_area,
	fruitapp::fruit::cut_geometry const &_cut_geometry,
	fruitapp::fruit::mesh_unique_ptr _cross_section)
:
	old_(
		_old),
	new_(
		_new),
	area_(
		_area),
	cut_geometry_(
		_cut_geometry),
	cross_section_(
		fcppt::move(
			_cross_section))
{
}

fruitapp::fruit::object const &
fruitapp::fruit::cut_context::old() const
{
	return
		old_;
}

fruitapp::fruit::cut_context::new_fruit_array const &
fruitapp::fruit::cut_context::new_fruits() const
{
	return
		new_;
}

fruitapp::fruit::area const
fruitapp::fruit::cut_context::area() const
{
	return
		area_;
}

fruitapp::fruit::cut_geometry const &
fruitapp::fruit::cut_context::cut_geometry() const
{
	return cut_geometry_;
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
