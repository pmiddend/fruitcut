#ifndef FRUITAPP_PROJECTION_MANAGER_PROJECTION_CHANGE_CALLBACK_HPP_INCLUDED
#define FRUITAPP_PROJECTION_MANAGER_PROJECTION_CHANGE_CALLBACK_HPP_INCLUDED

#include <fruitapp/projection_manager/projection_change_callback_fn.hpp>
#include <fcppt/function/object.hpp>

namespace fruitapp
{
namespace projection_manager
{
typedef
fcppt::function::object<projection_manager::projection_change_callback_fn>
projection_change_callback;
}
}

#endif

