#ifndef FRUITCUT_MESH_HPP_INCLUDED
#define FRUITCUT_MESH_HPP_INCLUDED

#include "triangle.hpp"
#include <vector>

namespace fruitcut
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

#endif
