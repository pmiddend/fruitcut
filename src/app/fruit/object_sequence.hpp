#ifndef FRUITCUT_APP_FRUIT_SEQUENCE_HPP_INCLUDED
#define FRUITCUT_APP_FRUIT_SEQUENCE_HPP_INCLUDED

#include "object.hpp"
#include "../../fruitlib/delayed_ptr_sequence.hpp"
#include <boost/ptr_container/ptr_list.hpp>

namespace fruitcut
{
namespace app
{
namespace fruit
{
typedef
fruitlib::delayed_ptr_sequence<object,boost::ptr_list>
object_sequence;
}
}
}

#endif