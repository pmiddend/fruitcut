#ifndef FRUITLIB_LOG_SCOPED_SEQUENCE_PTR_HPP_INCLUDED
#define FRUITLIB_LOG_SCOPED_SEQUENCE_PTR_HPP_INCLUDED

#include <fruitlib/log/scoped_sequence.hpp>
#include <fcppt/unique_ptr.hpp>


namespace fruitlib
{
namespace log
{
typedef
fcppt::unique_ptr<scoped_sequence>
scoped_sequence_ptr;
}
}

#endif
