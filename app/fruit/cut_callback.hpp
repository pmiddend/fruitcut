#ifndef FRUITCUT_APP_FRUIT_CUT_CALLBACK_HPP_INCLUDED
#define FRUITCUT_APP_FRUIT_CUT_CALLBACK_HPP_INCLUDED

#include "cut_callback_fn.hpp"
#include <boost/function.hpp>

namespace fruitcut
{
namespace app
{
namespace fruit
{
typedef
boost::function<cut_callback_fn>
cut_callback;
}
}
}

#endif
