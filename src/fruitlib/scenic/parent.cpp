#include <fruitlib/scenic/parent.hpp>


fruitlib::scenic::parent::parent(
	scenic::base &_object,
	scenic::depth const &_depth)
:
	object_(
		_object),
	depth_(
		_depth.get())
{
}

fruitlib::scenic::base &
fruitlib::scenic::parent::object() const
{
	return object_;
}

fruitlib::scenic::depth::value_type
fruitlib::scenic::parent::depth() const
{
	return depth_;
}
