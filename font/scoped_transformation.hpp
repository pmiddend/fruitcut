#ifndef FRUITCUT_FONT_SCOPED_TRANSFORMATION_HPP_INCLUDED
#define FRUITCUT_FONT_SCOPED_TRANSFORMATION_HPP_INCLUDED

#include "transform_callback.hpp"
#include "drawer_fwd.hpp"
#include <fcppt/noncopyable.hpp>

namespace fruitcut
{
namespace font
{
class scoped_transformation
{
FCPPT_NONCOPYABLE(
	scoped_transformation);
public:
	explicit
	scoped_transformation(
		drawer &,
		transform_callback const &);

	~scoped_transformation();
private:
	drawer &drawer_;
};
}
}

#endif
