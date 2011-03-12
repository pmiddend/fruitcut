#ifndef FRUITCUT_APP_FRUIT_CUT_CALLBACK_HPP_INCLUDED
#define FRUITCUT_APP_FRUIT_CUT_CALLBACK_HPP_INCLUDED

#include "cut_callback_fn.hpp"
#include <fcppt/function/object.hpp>

namespace fruitcut
{
namespace app
{
namespace fruit
{
typedef
fcppt::function::object<cut_callback_fn>
cut_callback;
}
}
}

#endif
