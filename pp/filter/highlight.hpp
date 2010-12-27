#ifndef FRUITCUT_PP_FILTER_HIGHLIGHT_HPP_INCLUDED
#define FRUITCUT_PP_FILTER_HIGHLIGHT_HPP_INCLUDED

#include "unary.hpp"
#include <sge/renderer/texture_ptr.hpp>
#include <sge/renderer/device_ptr.hpp>
#include <sge/renderer/target_ptr.hpp>
#include <sge/renderer/vertex_buffer_ptr.hpp>
#include <sge/renderer/dim2.hpp>
#include <sge/shader/object.hpp>

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
		sge::renderer::dim2 const &);

	sge::renderer::texture_ptr const
	apply(
		sge::renderer::texture_ptr);
private:
	sge::renderer::device_ptr renderer_;
	sge::renderer::texture_ptr texture_;
	sge::renderer::target_ptr target_;
	sge::shader::object shader_;
	sge::renderer::vertex_buffer_ptr quad_;
};
}
}
}

#endif