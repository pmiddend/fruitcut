#include "parent.hpp"

fruitcut::fruitlib::scenic::parent::parent(
	scenic::base &_object,
	scenic::depth const &_depth)
:
	object_(
		_object),
	depth_(
		_depth.get())
{
}

fruitcut::fruitlib::scenic::base &
fruitcut::fruitlib::scenic::parent::object() const
{
	return object_;
}

fruitcut::fruitlib::scenic::depth::value_type
fruitcut::fruitlib::scenic::parent::depth() const
{
	return depth_;
}
