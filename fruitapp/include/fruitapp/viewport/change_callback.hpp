#ifndef FRUITAPP_VIEWPORT_CHANGE_CALLBACK_HPP_INCLUDED
#define FRUITAPP_VIEWPORT_CHANGE_CALLBACK_HPP_INCLUDED

#include <fruitapp/viewport/change_callback_fn.hpp>
#include <fcppt/config/external_begin.hpp>
#include <functional>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
namespace viewport
{
typedef
std::function<viewport::change_callback_fn>
change_callback;
}
}

#endif

