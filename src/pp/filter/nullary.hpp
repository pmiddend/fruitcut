#ifndef FRUITCUT_PP_FILTER_NULLARY_HPP_INCLUDED
#define FRUITCUT_PP_FILTER_NULLARY_HPP_INCLUDED

#include "base.hpp"
#include "../texture/counted_instance.hpp"

namespace fruitcut
{
namespace pp
{
namespace filter
{
class nullary
:
	public base
{
public:
	virtual texture::counted_instance const
	apply() = 0;
};
}
}
}

#endif
