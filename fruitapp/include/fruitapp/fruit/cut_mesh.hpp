#ifndef FRUITCUT_FRUIT_APP_CUT_MESH_HPP_INCLUDED
#define FRUITCUT_FRUIT_APP_CUT_MESH_HPP_INCLUDED

#include <fruitapp/fruit/cut_mesh_result.hpp>
#include <fruitapp/fruit/mesh_fwd.hpp>
#include <fruitapp/fruit/plane.hpp>
#include <fcppt/preprocessor/pure.hpp>
#include <fcppt/config/external_begin.hpp>
#include <memory>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
namespace fruit
{
/// Cuts a mesh along a plane
std::unique_ptr<fruitapp::fruit::cut_mesh_result>
cut_mesh(
	fruitapp::fruit::mesh const &,
	fruitapp::fruit::plane const &)
FCPPT_PP_PURE;
}
}

#endif
