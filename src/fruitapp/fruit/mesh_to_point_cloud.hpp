#ifndef FRUITAPP_FRUIT_MESH_TO_POINT_CLOUD_HPP_INCLUDED
#define FRUITAPP_FRUIT_MESH_TO_POINT_CLOUD_HPP_INCLUDED

#include "point_cloud.hpp"
#include "mesh_fwd.hpp"

namespace fruitapp
{
namespace fruit
{
point_cloud const
mesh_to_point_cloud(
	mesh const &);
}
}

#endif
