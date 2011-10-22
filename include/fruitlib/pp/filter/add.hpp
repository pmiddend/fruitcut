#ifndef FRUITLIB_PP_FILTER_ADD_HPP_INCLUDED
#define FRUITLIB_PP_FILTER_ADD_HPP_INCLUDED

#include <fruitlib/pp/filter/binary.hpp>
#include <fruitlib/pp/filter/manager_fwd.hpp>
#include <fruitlib/pp/texture/counted_instance.hpp>
#include <fruitlib/pp/texture/manager_fwd.hpp>
#include <sge/renderer/device_fwd.hpp>
#include <sge/renderer/dim2.hpp>
#include <sge/shader/object_fwd.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/math/dim/basic_impl.hpp>


namespace fruitlib
{
namespace pp
{
namespace filter
{
class add
:
	public binary
{
FCPPT_NONCOPYABLE(
	add);
public:
	explicit
	add(
		sge::renderer::device &,
		filter::manager &,
		texture::manager &,
		sge::renderer::dim2 const &);

	texture::counted_instance const
	apply(
		texture::counted_instance,
		texture::counted_instance);

	~add();
private:
	sge::renderer::device &renderer_;
	filter::manager &filter_manager_;
	sge::renderer::dim2 dimension_;
	sge::shader::object &shader_;
	texture::manager &texture_manager_;
};
}
}
}

#endif
