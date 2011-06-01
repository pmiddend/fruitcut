#ifndef FRUITCUT_FRUITLIB_INPUT_OPTIONAL_MOUSE_AXIS_CALLBACK_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_INPUT_OPTIONAL_MOUSE_AXIS_CALLBACK_HPP_INCLUDED

#include <sge/input/mouse/axis_callback.hpp>
#include <fcppt/optional.hpp>

namespace fruitcut
{
namespace fruitlib
{
namespace input
{
typedef
fcppt::optional<sge::input::mouse::axis_callback>
optional_mouse_axis_callback;
}
}
}

#endif
