#ifndef FRUITAPP_SHADOW_MAP_OBJECT_UNIQUE_PTR_HPP_INCLUDED
#define FRUITAPP_SHADOW_MAP_OBJECT_UNIQUE_PTR_HPP_INCLUDED

#include <fruitapp/shadow_map/object_fwd.hpp>
#include <fcppt/unique_ptr_impl.hpp>


namespace fruitapp
{
namespace shadow_map
{
typedef
fcppt::unique_ptr<fruitapp::shadow_map::object>
object_unique_ptr;
}
}

#endif
