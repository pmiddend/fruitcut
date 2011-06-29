#ifndef FRUITCUT_APP_FRUIT_MODEL_TO_MESH_HPP_INCLUDED
#define FRUITCUT_APP_FRUIT_MODEL_TO_MESH_HPP_INCLUDED

#include "mesh.hpp"
#include <sge/md3/object_fwd.hpp>

namespace fruitcut
{
namespace app
{
namespace fruit
{
mesh const
model_to_mesh(
	sge::md3::object const &);
}
}
}

#endif
