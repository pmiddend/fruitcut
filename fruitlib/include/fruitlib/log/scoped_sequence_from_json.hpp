#ifndef FRUITLIB_LOG_SCOPED_SEQUENCE_FROM_JSON_HPP_INCLUDED
#define FRUITLIB_LOG_SCOPED_SEQUENCE_FROM_JSON_HPP_INCLUDED

#include <fruitlib/log/scoped_sequence.hpp>
#include <sge/parse/json/array_fwd.hpp>
#include <fcppt/log/context_fwd.hpp>


namespace fruitlib
{
namespace log
{
scoped_sequence
scoped_sequence_from_json(
	fcppt::log::context &,
	sge::parse::json::array const &);
}
}

#endif
