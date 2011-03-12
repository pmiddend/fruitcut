#ifndef FRUITCUT_LOG_SCOPED_SEQUENCE_FROM_JSON_HPP_INCLUDED
#define FRUITCUT_LOG_SCOPED_SEQUENCE_FROM_JSON_HPP_INCLUDED

#include "scoped_sequence_ptr.hpp"
#include <fcppt/log/context_fwd.hpp>
#include <sge/parse/json/array_fwd.hpp>

namespace fruitcut
{
namespace log
{
scoped_sequence_ptr
scoped_sequence_from_json(
	fcppt::log::context &,
	sge::parse::json::array const &);
}
}

#endif
