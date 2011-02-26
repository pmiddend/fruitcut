#ifndef FRUITCUT_PP_FILTER_HIGHLIGHT_HPP_INCLUDED
#define FRUITCUT_PP_FILTER_HIGHLIGHT_HPP_INCLUDED

#include "../screen_vf/quad.hpp"
#include "../texture/counted_instance.hpp"
#include "../texture/manager_fwd.hpp"
#include "unary.hpp"
#include <sge/renderer/device_ptr.hpp>
#include <sge/renderer/dim2.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/shader/object.hpp>
#include <fcppt/math/dim/basic_impl.hpp>

namespace fruitcut
{
namespace pp
{
namespace filter
{
class highlight
:
	public unary
{
public:	
	explicit
	highlight(
		sge::renderer::device_ptr,
		texture::manager &,
		sge::renderer::dim2 const &,
		sge::renderer::scalar threshold);

	texture::counted_instance const
	apply(
		texture::counted_instance);
private:
	sge::renderer::device_ptr renderer_;
	texture::manager &texture_manager_;
	sge::renderer::dim2 texture_size_;
	sge::shader::object shader_;
	screen_vf::quad quad_;
};
}
}
}

#endif
