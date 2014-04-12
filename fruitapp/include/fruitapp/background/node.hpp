#ifndef FRUITAPP_BACKGROUND_NODE_HPP_INCLUDED
#define FRUITAPP_BACKGROUND_NODE_HPP_INCLUDED

#include <fruitapp/background/base_unique_ptr.hpp>
#include <fruitapp/background/repetitions.hpp>
#include <fruitapp/background/use_ffp.hpp>
#include <fruitapp/projection_manager/object_fwd.hpp>
#include <fruitapp/shadow_map/optional_object_ref.hpp>
#include <fruitlib/texture_manager_fwd.hpp>
#include <fruitlib/scenic/node.hpp>
#include <fruitlib/scenic/optional_parent.hpp>
#include <fruitlib/scenic/events/render.hpp>
#include <sge/camera/base_fwd.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <sge/renderer/device/core_fwd.hpp>
#include <sge/shader/optional_context_ref.hpp>
#include <sge/shader/optional_context_ref.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
namespace background
{
class node
:
	public fruitlib::scenic::node<node>
{
FCPPT_NONCOPYABLE(
	node);
public:
	typedef
	boost::mpl::vector1
	<
		fruitlib::scenic::events::render
	>
	scene_reactions;

	node(
		fruitlib::scenic::optional_parent const &,
		fruitlib::texture_manager &,
		sge::renderer::device::core &,
		fruitapp::background::use_ffp const &,
		fruitapp::background::repetitions const &,
		sge::camera::base const &,
		fruitapp::projection_manager::object &,
		sge::shader::optional_context_ref const &,
		fruitapp::shadow_map::optional_object_ref const &);

	void
	react(
		fruitlib::scenic::events::render const &);

	~node();
private:
	fruitapp::background::base_unique_ptr const background_;
};
}
}

#endif
