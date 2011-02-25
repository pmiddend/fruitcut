#ifndef FRUITCUT_PP_FILTER_SSAA_HPP_INCLUDED
#define FRUITCUT_PP_FILTER_SSAA_HPP_INCLUDED

#include "unary.hpp"
#include "../texture/manager_fwd.hpp"
#include "../texture/counted_instance.hpp"
#include "../screen_vf/declaration_buffer_pair.hpp"
#include <sge/renderer/dim2.hpp>
#include <fcppt/math/dim/basic_impl.hpp>
#include <sge/shader/object.hpp>

namespace fruitcut
{
namespace pp
{
namespace filter
{
class ssaa
:
	public unary
{
public:
	explicit
	ssaa(
		sge::renderer::device_ptr,
		texture::manager &,
		sge::renderer::dim2 const &);

	texture::counted_instance const
	apply(
		texture::counted_instance);

	~ssaa();
private:
	sge::renderer::device_ptr renderer_;
	texture::manager &texture_manager_;
	sge::renderer::dim2 const texture_size_;
	sge::shader::object shader_;
	screen_vf::declaration_buffer_pair quad_;
};
}
}
}

#endif
