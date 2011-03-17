#ifndef FRUITCUT_APP_FRUIT_CALLBACKS_SPAWN_HPP_INCLUDED
#define FRUITCUT_APP_FRUIT_CALLBACKS_SPAWN_HPP_INCLUDED

#include "spawn_fn.hpp"
#include <fcppt/tr1/functional.hpp>

namespace fruitcut
{
namespace app
{
namespace fruit
{
namespace callbacks
{
typedef
std::tr1::function<spawn_fn>
spawn;
}
}
}
}

#endif
