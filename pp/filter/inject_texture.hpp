#ifndef FRUITCUT_PP_FILTER_INJECT_TEXTURE_HPP_INCLUDED
#define FRUITCUT_PP_FILTER_INJECT_TEXTURE_HPP_INCLUDED

#include "../texture/manager_fwd.hpp"
#include "../texture/counted_instance.hpp"
#include "nullary.hpp"
#include <sge/renderer/texture/planar_ptr.hpp>

namespace fruitcut
{
namespace pp
{
namespace filter
{
class inject_texture
:
	public nullary
{
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
