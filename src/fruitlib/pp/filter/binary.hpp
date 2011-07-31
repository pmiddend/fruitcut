#ifndef FRUITLIB_PP_FILTER_BINARY_HPP_INCLUDED
#define FRUITLIB_PP_FILTER_BINARY_HPP_INCLUDED

#include "base.hpp"
#include <fruitlib/pp/texture/counted_instance.hpp>

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
public:
	virtual texture::counted_instance const
	apply(
		texture::counted_instance,
		texture::counted_instance) = 0;
};
}
}
}

#endif
