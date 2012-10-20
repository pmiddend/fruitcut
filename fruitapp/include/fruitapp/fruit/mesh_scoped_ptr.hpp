#ifndef FRUITAPP_FRUIT_MESH_SCOPED_PTR_HPP_INCLUDED
#define FRUITAPP_FRUIT_MESH_SCOPED_PTR_HPP_INCLUDED

#include <fruitapp/fruit/mesh_fwd.hpp>
#include <fcppt/scoped_ptr.hpp>

namespace fruitapp
{
namespace fruit
{
typedef
fcppt::scoped_ptr<fruit::mesh>
mesh_scoped_ptr;
}
}

#endif

