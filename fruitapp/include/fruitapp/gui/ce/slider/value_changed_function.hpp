#ifndef FRUITAPP_GUI_CE_SLIDER_VALUE_CHANGED_FUNCTION_HPP_INCLUDED
#define FRUITAPP_GUI_CE_SLIDER_VALUE_CHANGED_FUNCTION_HPP_INCLUDED

#include <fruitapp/gui/ce/slider/value_changed_fn.hpp>
#include <fcppt/function_impl.hpp>


namespace fruitapp
{
namespace gui
{
namespace ce
{
namespace slider
{
typedef
fcppt::function<fruitapp::gui::ce::slider::value_changed_fn>
value_changed_function;
}
}
}
}

#endif
