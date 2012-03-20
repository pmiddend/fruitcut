#ifndef FRUITLIB_FONT_DRAWER_SCOPED_TRANSFORMATION_HPP_INCLUDED
#define FRUITLIB_FONT_DRAWER_SCOPED_TRANSFORMATION_HPP_INCLUDED

#include <fruitlib/font/drawer/object_fwd.hpp>
#include <fruitlib/font/drawer/transform_callback.hpp>
#include <fcppt/noncopyable.hpp>


namespace fruitlib
{
namespace font
{
namespace drawer
{
class scoped_transformation
{
FCPPT_NONCOPYABLE(
	scoped_transformation);
public:
	explicit
	scoped_transformation(
		object &,
		transform_callback const &);

	~scoped_transformation();
private:
	object &drawer_;
	transform_callback const old_callback_;
};
}
}
}

#endif
