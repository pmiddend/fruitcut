#ifndef FRUITCUT_FRUIT_APP_CUT_MESH_HPP_INCLUDED
#define FRUITCUT_FRUIT_APP_CUT_MESH_HPP_INCLUDED

#include <fruitapp/fruit/cut_mesh_result.hpp>
#include <fruitapp/fruit/mesh_fwd.hpp>
#include <fruitapp/fruit/plane.hpp>
#include <fcppt/unique_ptr.hpp>

namespace fruitapp
{
namespace fruit
{
fcppt::unique_ptr<fruit::cut_mesh_result>
cut_mesh(
	fruit::mesh const &,
	fruit::plane const &);
}
}

#endif
