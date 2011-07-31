#ifndef FRUITLIB_PP_FILTER_DESATURATE_HPP_INCLUDED
#define FRUITLIB_PP_FILTER_DESATURATE_HPP_INCLUDED

#include <fruitlib/pp/filter/unary.hpp>
#include <fruitlib/pp/filter/manager_fwd.hpp>
#include <fruitlib/pp/texture/manager_fwd.hpp>
#include <sge/renderer/device_fwd.hpp>
#include <sge/renderer/target_ptr.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/renderer/dim2.hpp>
#include <sge/shader/object_fwd.hpp>
#include <fcppt/math/dim/dim.hpp>
#include <fcppt/noncopyable.hpp>

namespace fruitlib
{
namespace pp
{
namespace filter
{
class desaturate
:
	public unary
{
FCPPT_NONCOPYABLE(
	desaturate);
public:
	explicit
	desaturate(
		sge::renderer::device &,
		filter::manager &,
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
	sge::renderer::device &renderer_;
	filter::manager &filter_manager_;
	texture::manager &texture_manager_;
	sge::renderer::dim2 texture_size_;
	sge::renderer::scalar factor_;
	sge::shader::object &shader_;
};
}
}
}

#endif
