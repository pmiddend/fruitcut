#ifndef FRUITCUT_APP_FRUIT_SHADOW_RENDER_NODE_HPP_INCLUDED
#define FRUITCUT_APP_FRUIT_SHADOW_RENDER_NODE_HPP_INCLUDED

#include "manager_fwd.hpp"
#include "../../fruitlib/scenic/node.hpp"
#include "../../fruitlib/scenic/parent_fwd.hpp"
#include "../../fruitlib/scenic/events/render_fwd.hpp"
#include <sge/renderer/device_fwd.hpp>
#include <sge/renderer/vertex_declaration_fwd.hpp>
#include <sge/renderer/matrix4.hpp>
#include <sge/camera/object_fwd.hpp>
#include <sge/shader/object.hpp>
#include <fcppt/math/matrix/matrix.hpp>
#include <fcppt/noncopyable.hpp>
#include <boost/mpl/vector/vector10.hpp>

namespace fruitcut
{
namespace app
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

	explicit
	shadow_render_node(
		fruitlib::scenic::parent const &,
		sge::renderer::device &,
		sge::renderer::vertex_declaration &,
		fruit::manager const &,
		sge::renderer::matrix4 const &);

	~shadow_render_node();

	void
	react(
		fruitlib::scenic::events::render const &);
private:
	sge::renderer::device &renderer_;
	sge::renderer::vertex_declaration &vertex_declaration_;
	fruit::manager const &manager_;
	sge::shader::object shader_;
	sge::renderer::matrix4 mvp_;
};
}
}
}

#endif
