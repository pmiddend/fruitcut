#ifndef FRUITAPP_FRUIT_MESH_TO_POINT_CLOUD_HPP_INCLUDED
#define FRUITAPP_FRUIT_MESH_TO_POINT_CLOUD_HPP_INCLUDED

#include <fruitapp/fruit/mesh_fwd.hpp>
#include <fruitapp/fruit/point_cloud.hpp>
#include <fcppt/preprocessor/const.hpp>

namespace fruitapp
{
namespace fruit
{
point_cloud const
mesh_to_point_cloud(
	mesh const &)
FCPPT_PP_CONST;
}
}

#endif
