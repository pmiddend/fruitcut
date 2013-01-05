#ifndef FRUITAPP_HIGHSCORE_CALLBACKS_ERROR_RECEIVED_HPP_INCLUDED
#define FRUITAPP_HIGHSCORE_CALLBACKS_ERROR_RECEIVED_HPP_INCLUDED

#include <fruitapp/highscore/callbacks/error_received_fn.hpp>
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
std::function<error_received_fn>
error_received;
}
}
}

#endif
