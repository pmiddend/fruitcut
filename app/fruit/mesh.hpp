#ifndef FRUITCUT_APP_FRUIT_MESH_HPP_INCLUDED
#define FRUITCUT_APP_FRUIT_MESH_HPP_INCLUDED

#include "triangle.hpp"
#include <vector>

namespace fruitcut
{
namespace app
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
}

#endif
