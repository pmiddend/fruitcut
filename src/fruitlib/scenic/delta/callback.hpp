#ifndef FRUITLIB_SCENIC_DELTA_CALLBACK_HPP_INCLUDED
#define FRUITLIB_SCENIC_DELTA_CALLBACK_HPP_INCLUDED

#include <fruitlib/scenic/delta/duration.hpp>
#include <fcppt/function/object.hpp>

namespace fruitlib
{
namespace scenic
{
namespace delta
{
typedef
fcppt::function::object<delta::duration()>
callback;
}
}
}

#endif
