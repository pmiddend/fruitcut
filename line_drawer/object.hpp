#ifndef FRUITCUT_LINE_DRAWER_OBJECT_HPP_INCLUDED
#define FRUITCUT_LINE_DRAWER_OBJECT_HPP_INCLUDED

#include "line_sequence.hpp"
#include <sge/renderer/vertex_buffer_ptr.hpp>
#include <sge/renderer/vertex_declaration_ptr.hpp>
#include <sge/renderer/device_ptr.hpp>
#include <fcppt/noncopyable.hpp>

namespace fruitcut
{
namespace line_drawer
{
class object
{
FCPPT_NONCOPYABLE(
	object);
public:
	explicit
	object(
		sge::renderer::device_ptr);

	line_sequence &
	lines();

	line_sequence const &
	lines() const;

	void
	update();

	void
	render();

	void
	render_screen_space();

	~object();
private:
	sge::renderer::device_ptr renderer_;
	sge::renderer::vertex_declaration_ptr vertex_declaration_;
	sge::renderer::vertex_buffer_ptr vb_;
	line_sequence lines_;
};
}
}

#endif
