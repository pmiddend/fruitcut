#ifndef FRUITCUT_APP_HIGHSCORE_CALLBACKS_RANK_RECEIVED_HPP_INCLUDED
#define FRUITCUT_APP_HIGHSCORE_CALLBACKS_RANK_RECEIVED_HPP_INCLUDED

#include "rank_received_fn.hpp"
#include <fcppt/function/object.hpp>

namespace fruitcut
{
namespace app
{
namespace highscore
{
namespace callbacks
{
typedef
fcppt::function::object<callbacks::rank_received_fn>
rank_received;
}
}
}
}

#endif
