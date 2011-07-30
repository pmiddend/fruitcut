#ifndef FRUITCUT_FRUITLIB_FONT_SCALE_ANIMATION_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_FONT_SCALE_ANIMATION_HPP_INCLUDED

#include "../animation.hpp"
#include "scale.hpp"
#include <fcppt/chrono/milliseconds.hpp>

namespace fruitcut
{
namespace fruitlib
{
namespace font
{
typedef
fruitlib::animation<font::scale::value_type,fcppt::chrono::milliseconds>
scale_animation;
}
}
}

#endif
