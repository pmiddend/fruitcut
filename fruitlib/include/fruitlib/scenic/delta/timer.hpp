#ifndef FRUITLIB_SCENIC_DELTA_TIMER_HPP_INCLUDED
#define FRUITLIB_SCENIC_DELTA_TIMER_HPP_INCLUDED

#include <fruitlib/scenic/delta/clock.hpp>
#include <sge/timer/basic.hpp>


namespace fruitlib
{
namespace scenic
{
namespace delta
{
typedef
sge::timer::basic<delta::clock>
timer;
}
}
}

#endif
