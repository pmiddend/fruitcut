#ifndef FRUITCUT_APP_FRUIT_REMOVE_CALLBACK_HPP_INCLUDED
#define FRUITCUT_APP_FRUIT_REMOVE_CALLBACK_HPP_INCLUDED

#include "remove_callback_fn.hpp"
#include <fcppt/tr1/functional.hpp>

namespace fruitcut
{
namespace app
{
namespace fruit
{
typedef
std::tr1::function<remove_callback_fn>
remove_callback;
}
}
}

#endif
