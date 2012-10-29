#ifndef FRUITAPP_SHADOW_MAP_OBJECT_SCOPED_PTR_HPP_INCLUDED
#define FRUITAPP_SHADOW_MAP_OBJECT_SCOPED_PTR_HPP_INCLUDED

#include <fcppt/scoped_ptr.hpp>
#include <fruitapp/shadow_map/object_fwd.hpp>

namespace fruitapp
{
namespace shadow_map
{
typedef
fcppt::scoped_ptr<fruitapp::shadow_map::object>
object_scoped_ptr;
}
}

#endif
