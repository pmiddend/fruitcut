#ifndef FRUITAPP_HIGHSCORE_ENTRY_SET_HPP_INCLUDED
#define FRUITAPP_HIGHSCORE_ENTRY_SET_HPP_INCLUDED

#include <fruitapp/highscore/entry.hpp>
#include <fcppt/config/external_begin.hpp>
#include <set>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
namespace highscore
{
typedef
std::set<highscore::entry>
entry_set;
}
}

#endif
