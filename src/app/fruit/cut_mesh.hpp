#ifndef FRUITCUT_FRUIT_APP_CUT_MESH_HPP_INCLUDED
#define FRUITCUT_FRUIT_APP_CUT_MESH_HPP_INCLUDED

#include "mesh.hpp"
#include "plane.hpp"
#include "box3.hpp"
#include "area.hpp"
#include <sge/renderer/vector3.hpp>
#include <fcppt/math/vector/basic_impl.hpp>

namespace fruitcut
{
namespace app
{
namespace fruit
{
void
cut_mesh(
	mesh const &,
	plane const &,
	mesh &,
	box3 &,
	fruit::area &area,
	sge::renderer::vector3 &barycenter);
}
}
}

#endif
