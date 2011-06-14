#ifndef FRUITCUT_FRUITLIB_PP_FILTER_HIGHLIGHT_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_PP_FILTER_HIGHLIGHT_HPP_INCLUDED

#include "../texture/counted_instance.hpp"
#include "../texture/manager_fwd.hpp"
#include "unary.hpp"
#include "manager_fwd.hpp"
#include <sge/renderer/device_fwd.hpp>
#include <sge/renderer/dim2.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/shader/object_fwd.hpp>
#include <fcppt/math/dim/dim.hpp>
#include <fcppt/filesystem/path.hpp>

namespace fruitcut
{
namespace fruitlib
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
		sge::renderer::device &,
		filter::manager &,
		texture::manager &,
		sge::renderer::dim2 const &,
		sge::renderer::scalar threshold);

	texture::counted_instance const
	apply(
		texture::counted_instance);
private:
	sge::renderer::device &renderer_;
	filter::manager &filter_manager_;
	texture::manager &texture_manager_;
	sge::renderer::dim2 texture_size_;
	sge::shader::object &shader_;
};
}
}
}
}

#endif