#include <fruitlib/exception.hpp>
#include <fruitlib/log/scoped.hpp>
#include <fcppt/text.hpp>
#include <fcppt/log/activate_levels.hpp>
#include <fcppt/log/context.hpp>
#include <fcppt/log/enabled_level_array.hpp>
#include <fcppt/log/level.hpp>
#include <fcppt/log/location.hpp>
#include <fcppt/log/object.hpp>
#include <fcppt/optional/to_exception.hpp>


fruitlib::log::scoped::scoped(
	fcppt::log::context &_context,
	fcppt::log::location const &_location,
	fcppt::log::level const &_new_level)
:
	object_(
		fcppt::optional::to_exception(
			_context.find(
				_location
			),
			[
				&_location
			]{
				return
					fruitlib::exception(
						FCPPT_TEXT("The logger at location ")+
						_location.string()+
						FCPPT_TEXT(" was not found")
					);
			}
		)
	),
	old_levels_(
		object_.enabled_levels()
	)
{
	fcppt::log::activate_levels(
		object_,
		_new_level);
}

fruitlib::log::scoped::~scoped()
{
	for(
		auto const level
		:
		old_levels_
	)
		object_.enable(
			level
		);
}
