#ifndef FRUITAPP_FRUIT_RENDERING_FRUIT_POINTER_SEQUENCE_HPP_INCLUDED
#define FRUITAPP_FRUIT_RENDERING_FRUIT_POINTER_SEQUENCE_HPP_INCLUDED

#include <fruitapp/fruit/object_fwd.hpp>
#include <fcppt/config/external_begin.hpp>
#include <vector>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
namespace fruit
{
namespace rendering
{
typedef
std::vector<fruitapp::fruit::object const *>
fruit_pointer_sequence;
}
}
}

#endif
