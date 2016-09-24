#include <fruitlib/log/scoped.hpp>
#include <fcppt/log/context.hpp>
#include <fcppt/log/level.hpp>
#include <fcppt/log/location.hpp>
#include <fcppt/log/optional_level.hpp>


fruitlib::log::scoped::scoped(
	fcppt::log::context &_context,
	fcppt::log::location const &_location,
	fcppt::log::level const _new_level
)
:
	context_{
		_context
	},
	location_{
		_location
	},
	old_level_(
		_context.get(
			_location
		)
	)
{
	context_.set(
		location_,
		fcppt::log::optional_level{
			_new_level
		}
	);
}

fruitlib::log::scoped::~scoped()
{
	context_.set(
		location_,
		old_level_
	);
}
