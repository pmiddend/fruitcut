#include "scoped.hpp"
#include <fcppt/log/context.hpp>
#include <fcppt/log/object.hpp>
#include <fcppt/exception.hpp>
#include <boost/foreach.hpp>

fruitcut::log::scoped::scoped(
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
		throw fcppt::exception(
			FCPPT_TEXT("The logger at location ")+
			_location.string()+
			FCPPT_TEXT(" was not found"));

	old_levels_ = 
		object_->enabled_levels();
}

fruitcut::log::scoped::~scoped()
{
	BOOST_FOREACH(
		fcppt::log::enabled_level_array::const_reference r,
		old_levels_)
		object_->enable(
			r);
}
