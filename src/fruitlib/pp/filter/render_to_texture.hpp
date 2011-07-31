#ifndef FRUITLIB_PP_FILTER_RENDER_TO_TEXTURE_HPP_INCLUDED
#define FRUITLIB_PP_FILTER_RENDER_TO_TEXTURE_HPP_INCLUDED

#include "nullary.hpp"
#include "../texture/counted_instance.hpp"
#include "../texture/manager_fwd.hpp"
#include "../texture/depth_stencil_format.hpp"
#include <sge/renderer/device_fwd.hpp>
#include <sge/renderer/target_ptr.hpp>
#include <sge/renderer/dim2.hpp>
#include <sge/renderer/state/list.hpp>
#include <fcppt/math/dim/basic_impl.hpp>
#include <fcppt/function/object.hpp>

namespace fruitlib
{
namespace pp
{
namespace filter
{
class render_to_texture
:
	public nullary
{
public:
	typedef
	fcppt::function::object<void ()>
	callback;

	explicit	
	render_to_texture(
		sge::renderer::device &,
		sge::renderer::state::list const &,
		texture::manager &,
		sge::renderer::dim2 const &,
		callback const &,
		texture::depth_stencil_format::type);

	texture::counted_instance const
	apply();

	~render_to_texture();
private:
	sge::renderer::device &renderer_;
	sge::renderer::state::list const state_list_;
	texture::manager &texture_manager_;
	sge::renderer::dim2 const texture_size_;
	callback const callback_;
	texture::depth_stencil_format::type depth_stencil_;
};
}
}
}

#endif
