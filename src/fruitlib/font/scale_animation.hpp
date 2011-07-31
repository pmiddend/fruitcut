#ifndef FRUITLIB_FONT_SCALE_ANIMATION_HPP_INCLUDED
#define FRUITLIB_FONT_SCALE_ANIMATION_HPP_INCLUDED

#include <fruitlib/animation.hpp>
#include "scale.hpp"
#include <fcppt/chrono/milliseconds.hpp>

namespace fruitlib
{
namespace font
{
typedef
fruitlib::animation<font::scale::value_type,fcppt::chrono::milliseconds>
scale_animation;
}
}

#endif
