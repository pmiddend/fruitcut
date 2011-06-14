#ifndef FRUITCUT_FRUITLIB_FONT_DRAWER_TRANSFORM_CALLBACK_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_FONT_DRAWER_TRANSFORM_CALLBACK_HPP_INCLUDED

#include "transform_callback_fn.hpp"
#include <fcppt/function/object.hpp>

namespace fruitcut
{
namespace fruitlib
{
namespace font
{
namespace drawer
{
typedef
fcppt::function::object<transform_callback_fn>
transform_callback;
}
}
}
}

#endif