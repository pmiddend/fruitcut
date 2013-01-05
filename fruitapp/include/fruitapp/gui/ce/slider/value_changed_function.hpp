#ifndef FRUITAPP_GUI_CE_SLIDER_VALUE_CHANGED_FUNCTION_HPP_INCLUDED
#define FRUITAPP_GUI_CE_SLIDER_VALUE_CHANGED_FUNCTION_HPP_INCLUDED

#include <fruitapp/gui/ce/slider/value_changed_fn.hpp>
#include <fcppt/config/external_begin.hpp>
#include <functional>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
namespace gui
{
namespace ce
{
namespace slider
{
typedef
std::function<fruitapp::gui::ce::slider::value_changed_fn>
value_changed_function;
}
}
}
}

#endif
