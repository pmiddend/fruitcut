#ifndef FRUITLIB_LOG_SCOPED_HPP_INCLUDED
#define FRUITLIB_LOG_SCOPED_HPP_INCLUDED

#include <fruitlib/detail/symbol.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/log/context_fwd.hpp>
#include <fcppt/log/level_fwd.hpp>
#include <fcppt/log/location.hpp>
#include <fcppt/log/optional_level.hpp>


namespace fruitlib
{
namespace log
{

class scoped
{
	FCPPT_NONCOPYABLE(
		scoped
	);
public:
	FRUITLIB_DETAIL_SYMBOL
	scoped(
		fcppt::log::context &,
		fcppt::log::location const &,
		fcppt::log::level
	);

	FRUITLIB_DETAIL_SYMBOL
	~scoped();
private:
	fcppt::log::context &context_;

	fcppt::log::location const location_;

	fcppt::log::optional_level const old_level_;
};

}
}

#endif
