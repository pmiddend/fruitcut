#ifndef FRUITCUT_APP_HIGHSCORE_CALLBACKS_ERROR_RECEIVED_HPP_INCLUDED
#define FRUITCUT_APP_HIGHSCORE_CALLBACKS_ERROR_RECEIVED_HPP_INCLUDED

#include "error_received_fn.hpp"
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
fcppt::function::object<error_received_fn>
error_received;
}
}
}
}

#endif
