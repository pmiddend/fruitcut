#ifndef FRUITAPP_FRUIT_BAN_DURATION_HPP_INCLUDED
#define FRUITAPP_FRUIT_BAN_DURATION_HPP_INCLUDED

#include <fruitapp/ingame_clock.hpp>
#include <fcppt/strong_typedef.hpp>


namespace fruitapp
{
namespace fruit
{
FCPPT_MAKE_STRONG_TYPEDEF(
	fruitapp::ingame_clock::duration,
	ban_duration);
}
}

#endif
