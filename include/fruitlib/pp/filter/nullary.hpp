#ifndef FRUITLIB_PP_FILTER_NULLARY_HPP_INCLUDED
#define FRUITLIB_PP_FILTER_NULLARY_HPP_INCLUDED

#include <fruitlib/pp/filter/base.hpp>
#include <fruitlib/pp/texture/counted_instance.hpp>
#include <fcppt/noncopyable.hpp>


namespace fruitlib
{
namespace pp
{
namespace filter
{
class nullary
:
	public base
{
FCPPT_NONCOPYABLE(
	nullary);
public:
	explicit
	nullary();

	virtual
	~nullary();

	virtual texture::counted_instance const
	apply() = 0;
};
}
}
}

#endif
