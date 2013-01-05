#ifndef FRUITAPP_SHADOW_MAP_OBJECT_UNIQUE_PTR_HPP_INCLUDED
#define FRUITAPP_SHADOW_MAP_OBJECT_UNIQUE_PTR_HPP_INCLUDED

#include <fruitapp/shadow_map/object_fwd.hpp>
#include <fcppt/config/external_begin.hpp>
#include <memory>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
namespace shadow_map
{
typedef
std::unique_ptr<fruitapp::shadow_map::object>
object_unique_ptr;
}
}

#endif
