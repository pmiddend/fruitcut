#ifndef FRUITAPP_FRUIT_CUT_MESH_HPP_INCLUDED
#define FRUITAPP_FRUIT_CUT_MESH_HPP_INCLUDED

#include <fruitapp/fruit/cut_mesh_result.hpp>
#include <fruitapp/fruit/mesh_fwd.hpp>
#include <fruitapp/fruit/plane.hpp>
#include <fcppt/unique_ptr.hpp>
#include <fcppt/preprocessor/pure.hpp>


namespace fruitapp
{
namespace fruit
{
/// Cuts a mesh along a plane
fcppt::unique_ptr<fruitapp::fruit::cut_mesh_result>
cut_mesh(
	fruitapp::fruit::mesh const &,
	fruitapp::fruit::plane const &)
FCPPT_PP_PURE;
}
}

#endif
