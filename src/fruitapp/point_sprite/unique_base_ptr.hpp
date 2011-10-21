#ifndef FRUITAPP_POINT_SPRITE_UNIQUE_BASE_PTR_HPP_INCLUDED
#define FRUITAPP_POINT_SPRITE_UNIQUE_BASE_PTR_HPP_INCLUDED

#include <fruitapp/point_sprite/base.hpp>
#include <fcppt/unique_ptr.hpp>


namespace fruitapp
{
namespace point_sprite
{
typedef
fcppt::unique_ptr<base>
unique_base_ptr;
}
}

#endif
