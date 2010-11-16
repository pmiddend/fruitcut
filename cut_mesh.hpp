#ifndef FRUITCUT_CUT_MESH_HPP_INCLUDED
#define FRUITCUT_CUT_MESH_HPP_INCLUDED

#include "mesh.hpp"
#include "plane.hpp"

namespace fruitcut
{
mesh const
cut_mesh(
	mesh const &,
	plane const &);
}

#endif
