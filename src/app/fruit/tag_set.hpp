#ifndef FRUITCUT_APP_FRUIT_TAG_SET_HPP_INCLUDED
#define FRUITCUT_APP_FRUIT_TAG_SET_HPP_INCLUDED

#include "tag.hpp"
#include <fcppt/tr1/unordered_set.hpp>

namespace fruitcut
{
namespace app
{
namespace fruit
{
typedef
std::tr1::unordered_set<fruit::tag>
tag_set;
}
}
}

#endif
