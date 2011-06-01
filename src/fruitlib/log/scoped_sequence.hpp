#ifndef FRUITCUT_FRUITLIB_LOG_SCOPED_SEQUENCE_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_LOG_SCOPED_SEQUENCE_HPP_INCLUDED

#include "scoped.hpp"
#include <boost/ptr_container/ptr_vector.hpp>

namespace fruitcut
{
namespace fruitlib
{
namespace log
{
typedef
boost::ptr_vector<scoped>
scoped_sequence;
}
}
}

#endif
