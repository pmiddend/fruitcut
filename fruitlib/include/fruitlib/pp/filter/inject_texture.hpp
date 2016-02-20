#ifndef FRUITLIB_PP_FILTER_INJECT_TEXTURE_HPP_INCLUDED
#define FRUITLIB_PP_FILTER_INJECT_TEXTURE_HPP_INCLUDED

#include <fruitlib/detail/symbol.hpp>
#include <fruitlib/pp/filter/nullary.hpp>
#include <fruitlib/pp/texture/counted_instance.hpp>
#include <fcppt/noncopyable.hpp>


namespace fruitlib
{
namespace pp
{
namespace filter
{
class inject_texture
:
	public nullary
{
FCPPT_NONCOPYABLE(
	inject_texture);
public:
	FRUITLIB_DETAIL_SYMBOL
	inject_texture();

	FRUITLIB_DETAIL_SYMBOL
	void
	texture(
		fruitlib::pp::texture::counted_instance const &);

	fruitlib::pp::texture::counted_instance const
	apply();

	FRUITLIB_DETAIL_SYMBOL
	~inject_texture();
private:
	fruitlib::pp::texture::counted_instance texture_counted_;
};
}
}
}

#endif
