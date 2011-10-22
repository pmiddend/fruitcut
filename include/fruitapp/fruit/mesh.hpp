#ifndef FRUITAPP_FRUIT_MESH_HPP_INCLUDED
#define FRUITAPP_FRUIT_MESH_HPP_INCLUDED

#include <fruitapp/fruit/triangle.hpp>
#include <fcppt/config/external_begin.hpp>
#include <vector>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
namespace fruit
{
class mesh
{
public:
	typedef
	std::vector<triangle>
	triangle_sequence;

	triangle_sequence triangles;
};
}
}

#endif
