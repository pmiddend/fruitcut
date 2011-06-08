#ifndef FRUITCUT_FRUITLIB_PP_FILTER_SSAA_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_PP_FILTER_SSAA_HPP_INCLUDED

#include "unary.hpp"
#include "manager_fwd.hpp"
#include "../texture/manager_fwd.hpp"
#include "../texture/counted_instance.hpp"
#include <sge/renderer/dim2.hpp>
#include <sge/renderer/device_fwd.hpp>
#include <sge/shader/object_fwd.hpp>
#include <fcppt/math/dim/dim.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/filesystem/path.hpp>

namespace fruitcut
{
namespace fruitlib
{
namespace pp
{
namespace filter
{
class ssaa
:
	public unary
{
FCPPT_NONCOPYABLE(
	ssaa);
public:
	explicit
	ssaa(
		sge::renderer::device &,
		filter::manager &,
		texture::manager &,
		sge::renderer::dim2 const &);

	texture::counted_instance const
	apply(
		texture::counted_instance);

	~ssaa();
private:
	sge::renderer::device &renderer_;
	filter::manager &filter_manager_;
	texture::manager &texture_manager_;
	sge::renderer::dim2 const texture_size_;
	sge::shader::object &shader_;
};
}
}
}
}

#endif
