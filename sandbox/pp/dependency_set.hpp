#ifndef FRUITCUT_SANDBOX_PP_DEPENDENCY_SET_HPP_INCLUDED
#define FRUITCUT_SANDBOX_PP_DEPENDENCY_SET_HPP_INCLUDED

#include <fcppt/string.hpp>
#include <boost/unordered/unordered_set.hpp>

namespace fruitcut
{
namespace sandbox
{
namespace pp
{
typedef
boost::unordered_set<fcppt::string>
dependency_set;
}
}
}

#endif
