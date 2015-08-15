#ifndef FRUITAPP_HIGHSCORE_CALLBACKS_RANK_RECEIVED_HPP_INCLUDED
#define FRUITAPP_HIGHSCORE_CALLBACKS_RANK_RECEIVED_HPP_INCLUDED

#include <fruitapp/highscore/callbacks/rank_received_fn.hpp>
#include <fcppt/function_impl.hpp>


namespace fruitapp
{
namespace highscore
{
namespace callbacks
{
typedef
fcppt::function<callbacks::rank_received_fn>
rank_received;
}
}
}

#endif
