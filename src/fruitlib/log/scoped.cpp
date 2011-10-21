#include <fruitlib/exception.hpp>
#include <fruitlib/log/scoped.hpp>
#include <fcppt/log/activate_levels.hpp>
#include <fcppt/log/context.hpp>
#include <fcppt/log/enabled_level_array.hpp>
#include <fcppt/log/level.hpp>
#include <fcppt/log/location.hpp>
#include <fcppt/log/object.hpp>
#include <fcppt/config/external_begin.hpp>
#include <iostream>
#include <fcppt/config/external_end.hpp>


fruitlib::log::scoped::scoped(
	fcppt::log::context &_context,
	fcppt::log::location const &_location,
	fcppt::log::level::type const &_new_level)
:
	object_(
		_context.find(
			_location)),
	old_levels_()
{
	if(!object_)
		throw fruitlib::exception(
			FCPPT_TEXT("The logger at location ")+
			_location.string()+
			FCPPT_TEXT(" was not found"));

	old_levels_ =
		object_->enabled_levels();

	fcppt::log::activate_levels(
		*object_,
		_new_level);
}

fruitlib::log::scoped::~scoped()
{
	for(
		fcppt::log::enabled_level_array::const_iterator r =
			old_levels_.begin();
		r != old_levels_.end();
		++r)
		object_->enable(
			*r);
}
