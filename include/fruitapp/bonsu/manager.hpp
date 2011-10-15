#ifndef FRUITAPP_BONSU_MANAGER_HPP_INCLUDED
#define FRUITAPP_BONSU_MANAGER_HPP_INCLUDED

#include <fruitapp/bonsu/instance/base.hpp>
#include <fruitapp/bonsu/texture_manager.hpp>
#include <fruitapp/bonsu/sprite/system.hpp>
#include <fruitapp/bonsu/instance_wrapper_fwd.hpp>
#include <fruitapp/bonsu/rectangle/manager.hpp>
#include <fruitlib/scenic/node.hpp>
#include <fruitlib/scenic/optional_parent.hpp>
#include <fruitlib/scenic/events/update.hpp>
#include <fruitlib/scenic/events/viewport_change.hpp>
#include <fruitlib/scenic/events/render.hpp>
#include <sge/renderer/device_fwd.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <sge/image2d/multi_loader_fwd.hpp>
#include <sge/timer/clocks/standard.hpp>
#include <sge/timer/basic.hpp>
#include <fcppt/scoped_ptr.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/ptr_container/ptr_list.hpp>
#include <vector>

namespace fruitapp
{
namespace bonsu
{
/**
	How are bonsu managed in fruitcut:

	Insert description here
 */
class manager
:
	public fruitlib::scenic::node<manager>
{
FCPPT_NONCOPYABLE(
	manager);
public:
	typedef
	boost::mpl::vector3
	<
		fruitlib::scenic::events::update,
		fruitlib::scenic::events::render,
		fruitlib::scenic::events::viewport_change
	>
	scene_reactions;

	explicit
	manager(
		fruitlib::scenic::optional_parent const &,
		sge::parse::json::object const &,
		sge::image2d::multi_loader &,
		sge::renderer::device &);

	~manager();

	void
	react(
		fruitlib::scenic::events::update const &);

	void
	react(
		fruitlib::scenic::events::render const &);

	void
	react(
		fruitlib::scenic::events::viewport_change const &);
private:
	friend class instance::base;

	typedef
	boost::ptr_list<bonsu::instance_wrapper>
	bonsu_list;

	sge::renderer::device &renderer_;
	bonsu::texture_manager texture_manager_;
	sprite::system sprite_system_;
	bonsu::scalar const rectangle_speed_;
	bonsu::scalar const rectangle_bounding_rect_height_;
	bonsu::scalar const rectangle_padding_left_;
	bonsu::scalar const rectangle_padding_top_;
	bonsu::scalar const rectangle_padding_middle_;
	bonsu::scalar const overlay_transparency_;
	sge::timer::basic<sge::timer::clocks::standard> frame_timer_;
	fcppt::scoped_ptr<rectangle::manager> rectangle_manager_;
	bonsu_list bonsu_;

	void
	insert(
		instance::base &);

	void
	erase(
		instance::base &);
};
}
}

#endif
