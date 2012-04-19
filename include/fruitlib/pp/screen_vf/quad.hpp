#ifndef FRUITLIB_PP_SCREEN_VF_QUAD_HPP_INCLUDED
#define FRUITLIB_PP_SCREEN_VF_QUAD_HPP_INCLUDED

#include <sge/renderer/device_fwd.hpp>
#include <sge/renderer/vertex_buffer_scoped_ptr.hpp>
#include <sge/renderer/vertex_declaration_scoped_ptr.hpp>
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
		sge::renderer::device &);

	explicit
	quad();

	void
	render();

	sge::renderer::vertex_declaration &
	vertex_declaration();

	sge::renderer::vertex_declaration const &
	vertex_declaration() const;

	~quad();
private:
	sge::renderer::device &renderer_;
	sge::renderer::vertex_declaration_scoped_ptr declaration_;
	sge::renderer::vertex_buffer_scoped_ptr buffer_;
};
}
}
}

#endif
