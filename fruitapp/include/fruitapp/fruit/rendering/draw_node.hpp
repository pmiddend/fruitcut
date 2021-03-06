#ifndef FRUITAPP_FRUIT_RENDERING_DRAW_NODE_HPP_INCLUDED
#define FRUITAPP_FRUIT_RENDERING_DRAW_NODE_HPP_INCLUDED

#include <fruitapp/fruit/manager_fwd.hpp>
#include <fruitapp/fruit/rendering/base_unique_ptr.hpp>
#include <fruitapp/graphics_settings/object_fwd.hpp>
#include <fruitapp/light/manager_fwd.hpp>
#include <fruitlib/scenic/node.hpp>
#include <fruitlib/scenic/events/render.hpp>
#include <sge/camera/base_fwd.hpp>
#include <sge/renderer/device/core_fwd.hpp>
#include <sge/shader/optional_context_ref.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
namespace fruit
{
namespace rendering
{
class draw_node
:
	public fruitlib::scenic::node<draw_node>
{
FCPPT_NONCOPYABLE(
	draw_node);
public:
	typedef
	boost::mpl::vector1
	<
		fruitlib::scenic::events::render
	>
	scene_reactions;

	draw_node(
		fruitlib::scenic::optional_parent const &,
		sge::renderer::device::core &,
		fruitapp::graphics_settings::object const &,
		sge::shader::optional_context_ref const &,
		fruitapp::fruit::manager const &,
		sge::camera::base const &,
		fruitapp::light::manager const &);

	void
	react(
		fruitlib::scenic::events::render const &);

	~draw_node();
private:
	fruitapp::fruit::rendering::base_unique_ptr const implementation_;
};
}
}
}

#endif
