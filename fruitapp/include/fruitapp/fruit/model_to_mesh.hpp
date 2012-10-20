#ifndef FRUITAPP_FRUIT_MODEL_TO_MESH_HPP_INCLUDED
#define FRUITAPP_FRUIT_MODEL_TO_MESH_HPP_INCLUDED

#include <fruitapp/fruit/mesh_unique_ptr.hpp>
#include <sge/model/md3/object_fwd.hpp>
#include <fcppt/preprocessor/const.hpp>

namespace fruitapp
{
namespace fruit
{
fruit::mesh_unique_ptr
model_to_mesh(
	sge::model::md3::object const &)
FCPPT_PP_CONST;
}
}

#endif
