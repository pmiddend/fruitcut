#ifndef FRUITLIB_PP_FILTER_BINARY_HPP_INCLUDED
#define FRUITLIB_PP_FILTER_BINARY_HPP_INCLUDED

#include <fruitlib/pp/filter/base.hpp>
#include <fruitlib/pp/texture/counted_instance.hpp>
#include <fcppt/noncopyable.hpp>


namespace fruitlib
{
namespace pp
{
namespace filter
{
class binary
:
	public base
{
FCPPT_NONCOPYABLE(
	binary);
public:
	explicit
	binary();

	virtual
	~binary();

	virtual texture::counted_instance const
	apply(
		texture::counted_instance,
		texture::counted_instance) = 0;
};
}
}
}

#endif
