#ifndef FRUITLIB_PP_DEPENDENCY_SET_HPP_INCLUDED
#define FRUITLIB_PP_DEPENDENCY_SET_HPP_INCLUDED

#include <fcppt/string.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/unordered_set.hpp>
#include <fcppt/config/external_end.hpp>


namespace fruitlib
{
namespace pp
{
typedef
boost::unordered_set<fcppt::string>
dependency_set;
}
}

#endif
