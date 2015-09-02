#ifndef FRUITAPP_FRUIT_OBJECT_UNIQUE_PTR_HPP_INCLUDED
#define FRUITAPP_FRUIT_OBJECT_UNIQUE_PTR_HPP_INCLUDED

#include <fruitapp/fruit/object_fwd.hpp>
#include <fcppt/unique_ptr_impl.hpp>


namespace fruitapp
{
namespace fruit
{
typedef
fcppt::unique_ptr<fruit::object>
object_unique_ptr;
}
}

#endif

