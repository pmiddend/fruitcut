#ifndef FRUITAPP_FRUIT_TAG_SET_HPP_INCLUDED
#define FRUITAPP_FRUIT_TAG_SET_HPP_INCLUDED

#include <fruitapp/fruit/tag.hpp>
#include <fcppt/tr1/unordered_set.hpp>


namespace fruitapp
{
namespace fruit
{
typedef
std::tr1::unordered_set<fruit::tag>
tag_set;
}
}

#endif
