#ifndef FRUITLIB_SCENIC_PARENT_HPP_INCLUDED
#define FRUITLIB_SCENIC_PARENT_HPP_INCLUDED

#include "base_fwd.hpp"
#include "depth.hpp"

namespace fruitlib
{
namespace scenic
{
class parent
{
public:
	explicit
	parent(
		scenic::base &,
		scenic::depth const &);

	scenic::base &
	object() const;

	scenic::depth::value_type
	depth() const;
private:
	scenic::base &object_;
	scenic::depth::value_type const depth_;
};
}
}

#endif
