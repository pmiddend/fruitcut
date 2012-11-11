#ifndef FRUITAPP_FRUIT_CUT_GEOMETRY_HPP_INCLUDED
#define FRUITAPP_FRUIT_CUT_GEOMETRY_HPP_INCLUDED

#include <fruitapp/fruit/cut_direction.hpp>
#include <fruitapp/fruit/cut_plane.hpp>


namespace fruitapp
{
namespace fruit
{
/**
\brief Represents the three-dimensional geometry of a single cut (intersection of a fruit with the sword)
*/
class cut_geometry
{
public:
	cut_geometry(
		fruitapp::fruit::cut_plane const &,
		fruitapp::fruit::cut_direction const &);

	fruitapp::fruit::cut_plane const &
	cut_plane() const;

	fruitapp::fruit::cut_direction const &
	cut_direction() const;
private:
	fruitapp::fruit::cut_plane cut_plane_;
	fruitapp::fruit::cut_direction cut_direction_;
};
}
}

#endif
