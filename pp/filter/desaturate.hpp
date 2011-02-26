#ifndef FRUITCUT_PP_FILTER_DESATURATE_HPP_INCLUDED
#define FRUITCUT_PP_FILTER_DESATURATE_HPP_INCLUDED

#include "unary.hpp"
#include "../texture/manager_fwd.hpp"
#include "../screen_vf/quad.hpp"
#include <sge/renderer/device_ptr.hpp>
#include <sge/renderer/target_ptr.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/renderer/dim2.hpp>
#include <sge/shader/object.hpp>
#include <fcppt/math/dim/basic_impl.hpp>

namespace fruitcut
{
namespace pp
{
namespace filter
{
class desaturate
:
	public unary
{
public:
	explicit
	desaturate(
		sge::renderer::device_ptr,
		texture::manager &,
		sge::renderer::dim2 const &,
		sge::renderer::scalar factor);

	void
	factor(
		sge::renderer::scalar);

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
