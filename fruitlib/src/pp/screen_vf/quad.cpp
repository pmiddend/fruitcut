#include <fruitlib/pp/screen_vf/format.hpp>
#include <fruitlib/pp/screen_vf/quad.hpp>
#include <fruitlib/pp/screen_vf/vertex_view.hpp>
#include <sge/renderer/lock_mode.hpp>
#include <sge/renderer/primitive_type.hpp>
#include <sge/renderer/size_type.hpp>
#include <sge/renderer/context/core.hpp>
#include <sge/renderer/device/core.hpp>
#include <sge/renderer/vertex/buffer.hpp>
#include <sge/renderer/vertex/buffer_parameters.hpp>
#include <sge/renderer/vertex/count.hpp>
#include <sge/renderer/vertex/declaration.hpp>
#include <sge/renderer/vertex/declaration_parameters.hpp>
#include <sge/renderer/vertex/first.hpp>
#include <sge/renderer/vertex/scoped_buffer.hpp>
#include <sge/renderer/vertex/scoped_declaration.hpp>
#include <sge/renderer/vertex/scoped_lock.hpp>
#include <sge/renderer/vf/vertex.hpp>
#include <sge/renderer/vf/dynamic/make_format.hpp>
#include <sge/renderer/vf/dynamic/part_index.hpp>
#include <fcppt/container/bitfield/object_impl.hpp>
#include <fcppt/math/vector/object_impl.hpp>


fruitlib::pp::screen_vf::quad::quad(
	sge::renderer::device::core &_renderer)
:
	renderer_(
		_renderer),
	declaration_(
		renderer_.create_vertex_declaration(
			sge::renderer::vertex::declaration_parameters(
				sge::renderer::vf::dynamic::make_format<fruitlib::pp::screen_vf::format>()))),
	buffer_(
		renderer_.create_vertex_buffer(
			sge::renderer::vertex::buffer_parameters(
				*declaration_,
				sge::renderer::vf::dynamic::part_index(
					0u),
				sge::renderer::vertex::count(
					6u),
				sge::renderer::resource_flags_field::null())))
{
	sge::renderer::vertex::scoped_lock const vblock(
		*buffer_,
		sge::renderer::lock_mode::writeonly);

	fruitlib::pp::screen_vf::vertex_view const vertices(
		vblock.value());

	fruitlib::pp::screen_vf::vertex_view::iterator vb_it(
		vertices.begin());

	// Left top
	(vb_it++)->set<fruitlib::pp::screen_vf::position>(
		fruitlib::pp::screen_vf::position::packed_type(
			-1.0f, 1.0f));

	// Left bottom
	(vb_it++)->set<fruitlib::pp::screen_vf::position>(
		fruitlib::pp::screen_vf::position::packed_type(
			-1.0f,-1.0f));

	// Right top
	(vb_it++)->set<fruitlib::pp::screen_vf::position>(
		fruitlib::pp::screen_vf::position::packed_type(
			1.0f,1.0f));

	// Right top
	(vb_it++)->set<fruitlib::pp::screen_vf::position>(
		fruitlib::pp::screen_vf::position::packed_type(
			1.0f,1.0f));

	// Left bottom
	(vb_it++)->set<fruitlib::pp::screen_vf::position>(
		fruitlib::pp::screen_vf::position::packed_type(
			-1.0f,-1.0f));

	// Right bottom
	(vb_it++)->set<fruitlib::pp::screen_vf::position>(
		fruitlib::pp::screen_vf::position::packed_type(
			1.0f,-1.0f));
}

void
fruitlib::pp::screen_vf::quad::render(
	sge::renderer::context::core &_context)
{
	sge::renderer::vertex::scoped_declaration const scoped_vd_(
		_context,
		*declaration_);

	sge::renderer::vertex::scoped_buffer const scoped_vb_(
		_context,
		*buffer_);

	_context.render_nonindexed(
		sge::renderer::vertex::first(
			0u),
		sge::renderer::vertex::count(
			buffer_->linear_size()),
		sge::renderer::primitive_type::triangle_list);
}

sge::renderer::vertex::declaration const &
fruitlib::pp::screen_vf::quad::vertex_declaration() const
{
	return
		*declaration_;
}

fruitlib::pp::screen_vf::quad::~quad()
{
}
