#ifndef FRUITAPP_FRUIT_SEQUENCE_HPP_INCLUDED
#define FRUITAPP_FRUIT_SEQUENCE_HPP_INCLUDED

#include <fruitapp/fruit/object.hpp>
#include <fruitlib/delayed_ptr_sequence.hpp>
#include <boost/ptr_container/ptr_list.hpp>

namespace fruitapp
{
namespace fruit
{
typedef
fruitlib::delayed_ptr_sequence<object,boost::ptr_list>
object_sequence;
}
}

#endif
