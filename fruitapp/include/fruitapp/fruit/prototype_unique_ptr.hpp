#ifndef FRUITAPP_FRUIT_PROTOTYPE_UNIQUE_PTR_HPP_INCLUDED
#define FRUITAPP_FRUIT_PROTOTYPE_UNIQUE_PTR_HPP_INCLUDED

#include <fruitapp/fruit/prototype_fwd.hpp>
#include <fcppt/config/external_begin.hpp>
#include <memory>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
namespace fruit
{
typedef
std::unique_ptr<fruit::prototype>
prototype_unique_ptr;
}
}

#endif

