#ifndef FRUITCUT_APP_POINT_SPRITE_UNIQUE_BASE_PTR_HPP_INCLUDED
#define FRUITCUT_APP_POINT_SPRITE_UNIQUE_BASE_PTR_HPP_INCLUDED

#include "base.hpp"
#include <fcppt/unique_ptr.hpp>

namespace fruitcut
{
namespace app
{
namespace point_sprite
{
typedef
fcppt::unique_ptr<base>
unique_base_ptr;
}
}
}

#endif
