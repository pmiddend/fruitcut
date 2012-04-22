#ifndef FRUITAPP_VIEWPORT_CHANGE_CALLBACK_HPP_INCLUDED
#define FRUITAPP_VIEWPORT_CHANGE_CALLBACK_HPP_INCLUDED

#include <fruitapp/viewport/change_callback_fn.hpp>
#include <fcppt/function/object.hpp>


namespace fruitapp
{
namespace viewport
{
typedef
fcppt::function::object<viewport::change_callback_fn>
change_callback;
}
}

#endif

