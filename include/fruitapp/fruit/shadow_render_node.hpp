#ifndef FRUITAPP_FRUIT_SHADOW_RENDER_NODE_HPP_INCLUDED
#define FRUITAPP_FRUIT_SHADOW_RENDER_NODE_HPP_INCLUDED

#include <fruitapp/shadow_mvp.hpp>
#include <fruitapp/fruit/manager_fwd.hpp>
#include <fruitlib/scenic/node.hpp>
#include <fruitlib/scenic/optional_parent.hpp>
#include <fruitlib/scenic/events/render.hpp>
#include <sge/camera/first_person/object_fwd.hpp>
#include <sge/renderer/matrix4.hpp>
#include <sge/renderer/vertex_declaration_fwd.hpp>
#include <sge/renderer/state/core/depth_stencil/object_scoped_ptr.hpp>
#include <sge/shader/context_fwd.hpp>
#include <sge/shader/pair.hpp>
#include <sge/shader/parameter/matrix.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/math/matrix/object_impl.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
namespace fruit
{
class shadow_render_node
:
	public fruitlib::scenic::node<shadow_render_node>
{
FCPPT_NONCOPYABLE(
	shadow_render_node);
public:
	typedef
	boost::mpl::vector1<fruitlib::scenic::events::render>
	scene_reactions;

	shadow_render_node(
		fruitlib::scenic::optional_parent const &,
		sge::shader::context &,
		sge::renderer::vertex_declaration &,
		fruitapp::fruit::manager const &,
		fruitapp::shadow_mvp const &);

	~shadow_render_node();

	void
	react(
		fruitlib::scenic::events::render const &);
private:
	sge::renderer::vertex_declaration &vertex_declaration_;
	fruitapp::fruit::manager const &manager_;
	sge::shader::pair shader_;
	sge::shader::parameter::matrix<sge::renderer::scalar,4,4> mvp_parameter_;
	fruitapp::shadow_mvp mvp_;
	sge::renderer::state::core::depth_stencil::object_scoped_ptr const depth_stencil_state_;
};
}
}

#endif
