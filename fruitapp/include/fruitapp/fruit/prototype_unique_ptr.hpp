#ifndef FRUITAPP_FRUIT_PROTOTYPE_UNIQUE_PTR_HPP_INCLUDED
#define FRUITAPP_FRUIT_PROTOTYPE_UNIQUE_PTR_HPP_INCLUDED

#include <fruitapp/fruit/prototype_fwd.hpp>
#include <fcppt/unique_ptr_impl.hpp>


namespace fruitapp
{
namespace fruit
{
typedef
fcppt::unique_ptr<fruit::prototype>
prototype_unique_ptr;
}
}

#endif

