#ifndef FRUITAPP_FRUIT_TAG_SET_HPP_INCLUDED
#define FRUITAPP_FRUIT_TAG_SET_HPP_INCLUDED

#include <fruitapp/fruit/tag.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/unordered_set.hpp>
#include <fcppt/config/external_end.hpp>


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
