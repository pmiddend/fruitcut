#ifndef FRUITCUT_APP_CUT_MESH_HPP_INCLUDED
#define FRUITCUT_APP_CUT_MESH_HPP_INCLUDED

#include "mesh.hpp"
#include "plane.hpp"
#include "box3.hpp"
#include <sge/renderer/vector3.hpp>
#include <fcppt/math/vector/basic_impl.hpp>

namespace fruitcut
{
namespace app
{
void
cut_mesh(
	mesh const &,
	plane const &,
	mesh &,
	box3 &,
	sge::renderer::vector3 &barycenter);
}
}

#endif
