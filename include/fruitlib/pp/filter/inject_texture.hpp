#ifndef FRUITLIB_PP_FILTER_INJECT_TEXTURE_HPP_INCLUDED
#define FRUITLIB_PP_FILTER_INJECT_TEXTURE_HPP_INCLUDED

#include <fruitlib/pp/filter/nullary.hpp>
#include <fruitlib/pp/texture/counted_instance.hpp>
#include <fruitlib/pp/texture/manager_fwd.hpp>
#include <sge/renderer/texture/planar_ptr.hpp>
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
	explicit
	inject_texture(
		texture::manager &);

	void
	texture(
		sge::renderer::texture::planar_ptr);

	texture::counted_instance const
	apply();

	~inject_texture();
private:
	texture::manager &texture_manager_;
	sge::renderer::texture::planar_ptr texture_;
};
}
}
}

#endif
