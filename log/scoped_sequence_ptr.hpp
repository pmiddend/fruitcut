#ifndef FRUITCUT_LOG_SCOPED_SEQUENCE_PTR_HPP_INCLUDED
#define FRUITCUT_LOG_SCOPED_SEQUENCE_PTR_HPP_INCLUDED

#include "scoped_sequence.hpp"
#include <fcppt/unique_ptr.hpp>

namespace fruitcut
{
namespace log
{
typedef
fcppt::unique_ptr<scoped_sequence>
scoped_sequence_ptr;
}
}

#endif