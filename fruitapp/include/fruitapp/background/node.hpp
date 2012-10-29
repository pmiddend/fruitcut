#ifndef FRUITAPP_BACKGROUND_NODE_HPP_INCLUDED
#define FRUITAPP_BACKGROUND_NODE_HPP_INCLUDED

#include <fruitlib/scenic/node.hpp>
#include <fruitlib/scenic/optional_parent.hpp>
#include <fruitlib/scenic/events/render.hpp>
#include <sge/shader/optional_context_ref.hpp>
#include <sge/renderer/device/core_fwd.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <sge/image2d/system_fwd.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <fruitapp/background/base_scoped_ptr.hpp>
#include <sge/camera/base_fwd.hpp>
#include <sge/shader/optional_context_ref.hpp>
#include <fruitapp/shadow_map/optional_object_ref.hpp>
#include <fruitapp/projection_manager/object_fwd.hpp>

namespace fruitapp
{
namespace background
{
class node
:
	public fruitlib::scenic::node<node>
{
public:
	typedef
	boost::mpl::vector1
	<
		fruitlib::scenic::events::render
	>
	scene_reactions;

	node(
		fruitlib::scenic::optional_parent const &,
		sge::image2d::system &,
		sge::renderer::device::core &,
		sge::parse::json::object const &,
		sge::camera::base const &,
		fruitapp::projection_manager::object &,
		sge::shader::optional_context_ref const &,
		fruitapp::shadow_map::optional_object_ref const &);

	void
	react(
		fruitlib::scenic::events::render const &);

	~node();
private:
	fruitapp::background::base_scoped_ptr const background_;
};
}
}

#endif
