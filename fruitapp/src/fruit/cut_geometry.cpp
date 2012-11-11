#include <fruitapp/fruit/cut_geometry.hpp>

fruitapp::fruit::cut_geometry::cut_geometry(
	fruitapp::fruit::cut_plane const &_cut_plane,
	fruitapp::fruit::cut_direction const &_cut_direction)
:
	cut_plane_(
		_cut_plane),
	cut_direction_(
		_cut_direction)
{
}

fruitapp::fruit::cut_plane const &
fruitapp::fruit::cut_geometry::cut_plane() const
{
	return
		cut_plane_;
}

fruitapp::fruit::cut_direction const &
fruitapp::fruit::cut_geometry::cut_direction() const
{
	return
		cut_direction_;
}
