#ifndef FRUITLIB_PP_FILTER_INJECT_TEXTURE_HPP_INCLUDED
#define FRUITLIB_PP_FILTER_INJECT_TEXTURE_HPP_INCLUDED

#include <fruitlib/pp/filter/nullary.hpp>
#include <fruitlib/pp/texture/counted_instance.hpp>
#include <fruitlib/pp/texture/manager_fwd.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/scoped_ptr.hpp>


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
	explicit
	inject_texture(
		fruitlib::pp::texture::manager &);

	void
	texture(
		fruitlib::pp::texture::counted_instance const &);

	fruitlib::pp::texture::counted_instance const
	apply();

	~inject_texture();
private:
	fruitlib::pp::texture::manager &texture_manager_;
	fruitlib::pp::texture::counted_instance texture_counted_;
};
}
}
}

#endif
