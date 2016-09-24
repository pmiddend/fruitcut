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
	public fruitlib::pp::filter::base
{
FCPPT_NONCOPYABLE(
	binary);
public:
	~binary()
	override;

	virtual fruitlib::pp::texture::counted_instance const
	apply(
		fruitlib::pp::texture::counted_instance,
		fruitlib::pp::texture::counted_instance) = 0;
protected:
	binary();
};
}
}
}

#endif
