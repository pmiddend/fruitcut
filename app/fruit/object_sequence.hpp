#ifndef FRUITCUT_APP_FRUIT_SEQUENCE_HPP_INCLUDED
#define FRUITCUT_APP_FRUIT_SEQUENCE_HPP_INCLUDED

#include "object.hpp"
#include <boost/ptr_container/ptr_list.hpp>

namespace fruitcut
{
namespace app
{
namespace fruit
{
typedef
boost::ptr_list<object>
object_sequence;
}
}
}

#endif
