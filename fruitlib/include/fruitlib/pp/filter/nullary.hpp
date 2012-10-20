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
	public fruitlib::pp::filter::base
{
FCPPT_NONCOPYABLE(
	nullary);
public:
	virtual
	~nullary();

	virtual fruitlib::pp::texture::counted_instance const
	apply() = 0;
protected:
	nullary();
};
}
}
}

#endif
