#ifndef FRUITLIB_PP_DEPENDENCY_SET_HPP_INCLUDED
#define FRUITLIB_PP_DEPENDENCY_SET_HPP_INCLUDED

#include <fcppt/string.hpp>
#include <fcppt/config/external_begin.hpp>
#include <unordered_set>
#include <fcppt/config/external_end.hpp>

namespace fruitlib
{
namespace pp
{
typedef
std::unordered_set<fcppt::string>
dependency_set;
}
}

#endif
