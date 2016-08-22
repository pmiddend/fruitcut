#ifndef FRUITAPP_FRUIT_OBJECT_SEQUENCE_HPP_INCLUDED
#define FRUITAPP_FRUIT_OBJECT_SEQUENCE_HPP_INCLUDED

#include <fruitapp/fruit/object.hpp>
#include <fruitlib/delayed_ptr_sequence.hpp>
#include <fcppt/config/external_begin.hpp>
#include <list>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
namespace fruit
{
typedef
fruitlib::delayed_ptr_sequence<object,std::list>
object_sequence;
}
}

#endif
