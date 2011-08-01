#ifndef FRUITAPP_INGAME_CLOCK_HPP_INCLUDED
#define FRUITAPP_INGAME_CLOCK_HPP_INCLUDED

#include <sge/timer/clocks/adjustable.hpp>
#include <sge/timer/clocks/standard.hpp>

namespace fruitapp
{
typedef
sge::timer::clocks::adjustable<sge::timer::clocks::standard>
ingame_clock;
}

#endif
