#ifndef FRUITLIB_PP_FILTER_UNARY_HPP_INCLUDED
#define FRUITLIB_PP_FILTER_UNARY_HPP_INCLUDED

#include <fruitlib/pp/filter/base.hpp>
#include <fruitlib/pp/texture/counted_instance.hpp>
#include <fcppt/noncopyable.hpp>


namespace fruitlib
{
namespace pp
{
namespace filter
{
class unary
:
	public base
{
FCPPT_NONCOPYABLE(
		unary);
public:
	explicit
	unary();

	virtual
	~unary();

	virtual texture::counted_instance const
	apply(
		texture::counted_instance) = 0;
};
}
}
}

#endif
