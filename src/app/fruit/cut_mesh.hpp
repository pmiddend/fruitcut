#ifndef FRUITCUT_FRUIT_APP_CUT_MESH_HPP_INCLUDED
#define FRUITCUT_FRUIT_APP_CUT_MESH_HPP_INCLUDED

#include "cut_mesh_result.hpp"
#include "mesh_fwd.hpp"
#include "plane.hpp"
#include <fcppt/unique_ptr.hpp>

namespace fruitcut
{
namespace app
{
namespace fruit
{
fcppt::unique_ptr<fruit::cut_mesh_result>
cut_mesh(
	fruit::mesh const &,
	fruit::plane const &);
}
}
}

#endif