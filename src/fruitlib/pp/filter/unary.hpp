#ifndef FRUITCUT_FRUITLIB_PP_FILTER_UNARY_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_PP_FILTER_UNARY_HPP_INCLUDED

#include "base.hpp"
#include "../texture/counted_instance.hpp"

namespace fruitcut
{
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
public:
	virtual texture::counted_instance const
	apply(
		texture::counted_instance) = 0;
};
}
}
}
}

#endif
