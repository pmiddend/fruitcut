#ifndef FRUITAPP_INGAME_TIMER_HPP_INCLUDED
#define FRUITAPP_INGAME_TIMER_HPP_INCLUDED

#include "ingame_clock.hpp"
#include <sge/timer/basic.hpp>

namespace fruitapp
{
typedef
sge::timer::basic<fruitapp::ingame_clock>
ingame_timer;
}

#endif
