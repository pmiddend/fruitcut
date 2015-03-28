#ifndef FRUITLIB_SCENIC_PARENT_HPP_INCLUDED
#define FRUITLIB_SCENIC_PARENT_HPP_INCLUDED

#include <fruitlib/detail/symbol.hpp>
#include <fruitlib/scenic/base_fwd.hpp>
#include <fruitlib/scenic/depth.hpp>
#include <fcppt/nonassignable.hpp>


namespace fruitlib
{
namespace scenic
{
class parent
{
FCPPT_NONASSIGNABLE(
	parent);
public:
	FRUITLIB_DETAIL_SYMBOL
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
