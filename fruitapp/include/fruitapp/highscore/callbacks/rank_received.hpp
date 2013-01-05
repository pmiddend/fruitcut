#ifndef FRUITAPP_HIGHSCORE_CALLBACKS_RANK_RECEIVED_HPP_INCLUDED
#define FRUITAPP_HIGHSCORE_CALLBACKS_RANK_RECEIVED_HPP_INCLUDED

#include <fruitapp/highscore/callbacks/rank_received_fn.hpp>
#include <fcppt/config/external_begin.hpp>
#include <functional>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
namespace highscore
{
namespace callbacks
{
typedef
std::function<callbacks::rank_received_fn>
rank_received;
}
}
}

#endif
