#ifndef FRUITLIB_PP_DEPENDENCY_SET_HPP_INCLUDED
#define FRUITLIB_PP_DEPENDENCY_SET_HPP_INCLUDED

#include <fcppt/string.hpp>
#include <fcppt/tr1/unordered_set.hpp>

namespace fruitlib
{
namespace pp
{
typedef
std::tr1::unordered_set<fcppt::string>
dependency_set;
}
}

#endif
