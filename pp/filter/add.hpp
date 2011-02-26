#ifndef FRUITCUT_PP_FILTER_ADD_HPP_INCLUDED
#define FRUITCUT_PP_FILTER_ADD_HPP_INCLUDED

#include "binary.hpp"
#include "../screen_vf/quad.hpp"
#include "../texture/counted_instance.hpp"
#include "../texture/manager_fwd.hpp"
#include <sge/renderer/dim2.hpp>
#include <sge/shader/object.hpp>
#include <fcppt/math/dim/basic_impl.hpp>

namespace fruitcut
{
namespace pp
{
namespace filter
{
class add
:
	public binary
{
public:
	explicit
	add(
		sge::renderer::device_ptr,
		texture::manager &,
		sge::renderer::dim2 const &);

	texture::counted_instance const
	apply(
		texture::counted_instance,
		texture::counted_instance);

	~add();
private:
	sge::renderer::device_ptr renderer_;
	sge::renderer::dim2 dimension_;
	sge::shader::object shader_;
	screen_vf::quad quad_;
	texture::manager &texture_manager_;
};
}
}
}

#endif
