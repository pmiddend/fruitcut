#ifndef FRUITLIB_LOG_SCOPED_SEQUENCE_PTR_HPP_INCLUDED
#define FRUITLIB_LOG_SCOPED_SEQUENCE_PTR_HPP_INCLUDED

#include <fruitlib/log/scoped_sequence.hpp>
#include <fcppt/config/external_begin.hpp>
#include <memory>
#include <fcppt/config/external_end.hpp>


namespace fruitlib
{
namespace log
{
typedef
std::unique_ptr<scoped_sequence>
scoped_sequence_ptr;
}
}

#endif
