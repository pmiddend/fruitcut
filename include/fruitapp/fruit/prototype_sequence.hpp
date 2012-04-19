#ifndef FRUITAPP_FRUIT_PROTOTYPE_SEQUENCE_HPP_INCLUDED
#define FRUITAPP_FRUIT_PROTOTYPE_SEQUENCE_HPP_INCLUDED

#include <fruitapp/fruit/prototype.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
namespace fruit
{
typedef
boost::ptr_vector<prototype>
prototype_sequence;
}
}

#endif
