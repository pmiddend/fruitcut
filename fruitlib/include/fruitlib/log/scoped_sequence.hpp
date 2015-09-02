#ifndef FRUITLIB_LOG_SCOPED_SEQUENCE_HPP_INCLUDED
#define FRUITLIB_LOG_SCOPED_SEQUENCE_HPP_INCLUDED

#include <fruitlib/log/scoped.hpp>
#include <fcppt/unique_ptr.hpp>
#include <fcppt/config/external_begin.hpp>
#include <vector>
#include <fcppt/config/external_end.hpp>


namespace fruitlib
{
namespace log
{
typedef
std::vector<
	fcppt::unique_ptr<
		scoped
	>
>
scoped_sequence;
}
}

#endif
