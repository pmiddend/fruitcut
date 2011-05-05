#ifndef FRUITCUT_INPUT_OPTIONAL_MOUSE_BUTTON_CALLBACK_HPP_INCLUDED
#define FRUITCUT_INPUT_OPTIONAL_MOUSE_BUTTON_CALLBACK_HPP_INCLUDED

#include <sge/input/mouse/button_callback.hpp>
#include <fcppt/optional.hpp>

namespace fruitcut
{
namespace input
{
typedef
fcppt::optional<sge::input::mouse::button_callback>
optional_mouse_button_callback;
}
}

#endif
