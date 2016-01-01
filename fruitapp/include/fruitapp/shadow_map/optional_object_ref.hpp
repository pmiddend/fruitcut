#ifndef FRUITAPP_SHADOW_MAP_OPTIONAL_OBJECT_REF_HPP_INCLUDED
#define FRUITAPP_SHADOW_MAP_OPTIONAL_OBJECT_REF_HPP_INCLUDED

#include <fruitapp/shadow_map/object_fwd.hpp>
#include <fcppt/optional/object_fwd.hpp>


namespace fruitapp
{
namespace shadow_map
{
typedef
fcppt::optional::object<fruitapp::shadow_map::object &>
optional_object_ref;
}
}

#endif
