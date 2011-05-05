#ifndef FRUITCUT_APP_FRUIT_CALLBACKS_CUT_HPP_INCLUDED
#define FRUITCUT_APP_FRUIT_CALLBACKS_CUT_HPP_INCLUDED

#include "cut_fn.hpp"
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
std::tr1::function<cut_fn>
cut;
}
}
}
}

#endif
