#ifndef FRUITLIB_PP_SCREEN_VF_QUAD_HPP_INCLUDED
#define FRUITLIB_PP_SCREEN_VF_QUAD_HPP_INCLUDED

#include <sge/renderer/vertex/buffer_unique_ptr.hpp>
#include <sge/renderer/vertex/declaration_fwd.hpp>
#include <sge/renderer/vertex/declaration_unique_ptr.hpp>
#include <sge/renderer/context/core_fwd.hpp>
#include <sge/renderer/device/core_fwd.hpp>
#include <fcppt/noncopyable.hpp>


namespace fruitlib
{
namespace pp
{
namespace screen_vf
{
class quad
{
FCPPT_NONCOPYABLE(
	quad);
public:
	explicit
	quad(
		sge::renderer::device::core &);

	quad();

	void
	render(
		sge::renderer::context::core &);

	sge::renderer::vertex::declaration const &
	vertex_declaration() const;

	~quad();
private:
	sge::renderer::device::core &renderer_;
	sge::renderer::vertex::declaration_unique_ptr const declaration_;
	sge::renderer::vertex::buffer_unique_ptr const buffer_;
};
}
}
}

#endif
