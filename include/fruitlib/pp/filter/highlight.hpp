#ifndef FRUITLIB_PP_FILTER_HIGHLIGHT_HPP_INCLUDED
#define FRUITLIB_PP_FILTER_HIGHLIGHT_HPP_INCLUDED

#include <fruitlib/pp/filter/manager_fwd.hpp>
#include <fruitlib/pp/filter/unary.hpp>
#include <fruitlib/pp/texture/counted_instance.hpp>
#include <fruitlib/pp/texture/manager_fwd.hpp>
#include <sge/renderer/device_fwd.hpp>
#include <sge/renderer/dim2.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/shader/object_fwd.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/math/dim/object_impl.hpp>


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
FCPPT_NONCOPYABLE(
	highlight);
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

	~highlight();
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

#endif
