#ifndef FRUITAPP_FRUIT_TAG_SET_HPP_INCLUDED
#define FRUITAPP_FRUIT_TAG_SET_HPP_INCLUDED

#include <fruitapp/fruit/tag.hpp>
#include <fcppt/config/external_begin.hpp>
#include <unordered_set>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
namespace fruit
{
typedef
std::unordered_set<fruit::tag>
tag_set;
}
}

#endif
