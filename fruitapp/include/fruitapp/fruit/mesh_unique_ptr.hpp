#ifndef FRUITAPP_FRUIT_MESH_UNIQUE_PTR_HPP_INCLUDED
#define FRUITAPP_FRUIT_MESH_UNIQUE_PTR_HPP_INCLUDED

#include <fruitapp/fruit/mesh_fwd.hpp>
#include <fcppt/unique_ptr.hpp>


namespace fruitapp
{
namespace fruit
{
typedef
fcppt::unique_ptr<fruit::mesh>
mesh_unique_ptr;
}
}

#endif

