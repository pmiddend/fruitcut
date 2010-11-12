#ifndef FRUITCUT_INPUT_OPTIONAL_KEY_CALLBACK_HPP_INCLUDED
#define FRUITCUT_INPUT_OPTIONAL_KEY_CALLBACK_HPP_INCLUDED

#include <sge/input/keyboard/key_callback.hpp>
#include <fcppt/optional.hpp>

namespace fruitcut
{
namespace input
{
typedef
fcppt::optional<sge::input::keyboard::key_callback>
optional_key_callback;
}
}

#endif
