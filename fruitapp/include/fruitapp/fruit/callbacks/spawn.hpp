#ifndef FRUITAPP_FRUIT_CALLBACKS_SPAWN_HPP_INCLUDED
#define FRUITAPP_FRUIT_CALLBACKS_SPAWN_HPP_INCLUDED

#include <fruitapp/fruit/callbacks/spawn_fn.hpp>
#include <fcppt/config/external_begin.hpp>
#include <functional>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
namespace fruit
{
namespace callbacks
{
typedef
std::function<spawn_fn>
spawn;
}
}
}

#endif
