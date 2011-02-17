#ifndef FRUITCUT_FONT_COLOR_ANIMATION_HPP_INCLUDED
#define FRUITCUT_FONT_COLOR_ANIMATION_HPP_INCLUDED

#include "../animation.hpp"
#include "color.hpp"
// Since animation needs a sge::time::funit "module" we include the
// neccessary operators here (least surprise)
#include <mizuiro/color/operators.hpp>
#include <mizuiro/color/object.hpp>

namespace fruitcut
{
namespace font
{
typedef
animation<color>
color_animation;
}
}

#endif
