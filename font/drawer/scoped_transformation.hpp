#ifndef FRUITCUT_FONT_DRAWER_SCOPED_TRANSFORMATION_HPP_INCLUDED
#define FRUITCUT_FONT_DRAWER_SCOPED_TRANSFORMATION_HPP_INCLUDED

#include "transform_callback.hpp"
#include "object_fwd.hpp"
#include <fcppt/noncopyable.hpp>

namespace fruitcut
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
