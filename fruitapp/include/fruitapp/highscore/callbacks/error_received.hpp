#ifndef FRUITAPP_HIGHSCORE_CALLBACKS_ERROR_RECEIVED_HPP_INCLUDED
#define FRUITAPP_HIGHSCORE_CALLBACKS_ERROR_RECEIVED_HPP_INCLUDED

#include <fruitapp/highscore/callbacks/error_received_fn.hpp>
#include <fcppt/function_impl.hpp>


namespace fruitapp
{
namespace highscore
{
namespace callbacks
{
typedef
fcppt::function<error_received_fn>
error_received;
}
}
}

#endif
