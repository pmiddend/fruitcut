#ifndef FRUITLIB_LOG_SCOPED_SEQUENCE_HPP_INCLUDED
#define FRUITLIB_LOG_SCOPED_SEQUENCE_HPP_INCLUDED

#include <fruitlib/log/scoped.hpp>
#include <fcppt/config/external_begin.hpp>
#include <memory>
#include <vector>
#include <fcppt/config/external_end.hpp>


namespace fruitlib
{
namespace log
{
typedef
std::vector<
	std::unique_ptr<
		scoped
	>
>
scoped_sequence;
}
}

#endif
