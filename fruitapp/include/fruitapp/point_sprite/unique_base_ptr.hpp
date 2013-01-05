#ifndef FRUITAPP_POINT_SPRITE_UNIQUE_BASE_PTR_HPP_INCLUDED
#define FRUITAPP_POINT_SPRITE_UNIQUE_BASE_PTR_HPP_INCLUDED

#include <fruitapp/point_sprite/base.hpp>
#include <fcppt/config/external_begin.hpp>
#include <memory>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
namespace point_sprite
{
typedef
std::unique_ptr<base>
unique_base_ptr;
}
}

#endif
