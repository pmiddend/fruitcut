#ifndef FRUITLIB_SCENIC_DELTA_CALLBACK_HPP_INCLUDED
#define FRUITLIB_SCENIC_DELTA_CALLBACK_HPP_INCLUDED

#include <fruitlib/scenic/delta/duration.hpp>
#include <fcppt/config/external_begin.hpp>
#include <functional>
#include <fcppt/config/external_end.hpp>


namespace fruitlib
{
namespace scenic
{
namespace delta
{
typedef
std::function<delta::duration()>
callback;
}
}
}

#endif
