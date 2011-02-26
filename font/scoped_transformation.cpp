#include "scoped_transformation.hpp"
#include "drawer.hpp"
#include <boost/spirit/home/phoenix/core.hpp>

fruitcut::font::scoped_transformation::scoped_transformation(
	drawer &_drawer,
	transform_callback const &_callback)
:
	drawer_(
		_drawer)
{
	drawer_.transform_callback(
		_callback);
}

fruitcut::font::scoped_transformation::~scoped_transformation()
{
	drawer_.transform_callback(
		boost::phoenix::arg_names::arg2);
}
