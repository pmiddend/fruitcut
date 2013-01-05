#ifndef FRUITAPP_FRUIT_CALLBACKS_CUT_HPP_INCLUDED
#define FRUITAPP_FRUIT_CALLBACKS_CUT_HPP_INCLUDED

#include <fruitapp/fruit/callbacks/cut_fn.hpp>
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
std::function<cut_fn>
cut;
}
}
}

#endif
