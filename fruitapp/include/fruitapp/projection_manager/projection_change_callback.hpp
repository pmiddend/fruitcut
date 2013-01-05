#ifndef FRUITAPP_PROJECTION_MANAGER_PROJECTION_CHANGE_CALLBACK_HPP_INCLUDED
#define FRUITAPP_PROJECTION_MANAGER_PROJECTION_CHANGE_CALLBACK_HPP_INCLUDED

#include <fruitapp/projection_manager/projection_change_callback_fn.hpp>
#include <fcppt/config/external_begin.hpp>
#include <functional>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
namespace projection_manager
{
typedef
std::function<projection_manager::projection_change_callback_fn>
projection_change_callback;
}
}

#endif

