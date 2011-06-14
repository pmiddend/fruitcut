#ifndef FRUITCUT_FRUITLIB_PP_FILTER_ADD_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_PP_FILTER_ADD_HPP_INCLUDED

#include "binary.hpp"
#include "manager_fwd.hpp"
#include "../texture/counted_instance.hpp"
#include "../texture/manager_fwd.hpp"
#include <sge/renderer/dim2.hpp>
#include <sge/shader/shader.hpp>
#include <fcppt/math/dim/dim.hpp>
#include <fcppt/noncopyable.hpp>

namespace fruitcut
{
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
}

#endif