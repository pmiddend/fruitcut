#ifndef FRUITAPP_FRUIT_MESH_HPP_INCLUDED
#define FRUITAPP_FRUIT_MESH_HPP_INCLUDED

#include "triangle.hpp"
#include <vector>

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
