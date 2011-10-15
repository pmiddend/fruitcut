#ifndef FRUITAPP_FRUIT_TAG_SET_HPP_INCLUDED
#define FRUITAPP_FRUIT_TAG_SET_HPP_INCLUDED

#include <fruitapp/fruit/tag.hpp>
#include <boost/unordered_set.hpp>

namespace fruitapp
{
namespace fruit
{
typedef
boost::unordered_set<fruit::tag>
tag_set;
}
}

#endif
