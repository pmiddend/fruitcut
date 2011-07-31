#ifndef FRUITAPP_FRUIT_MANAGER_HPP_INCLUDED
#define FRUITAPP_FRUIT_MANAGER_HPP_INCLUDED

#include "callbacks/cut.hpp"
#include "callbacks/cut_fn.hpp"
#include "callbacks/remove.hpp"
#include "callbacks/remove_fn.hpp"
#include "callbacks/spawn.hpp"
#include "callbacks/spawn_fn.hpp"
#include "prototype.hpp"
#include "prototype_sequence.hpp"
#include "object_sequence.hpp"
#include "object_fwd.hpp"
#include "plane.hpp"
#include "../../fruitlib/scenic/node.hpp"
#include "../../fruitlib/scenic/optional_parent.hpp"
#include "../../fruitlib/scenic/events/update_fwd.hpp"
#include "../../fruitlib/scenic/events/render_fwd.hpp"
#include "../../fruitlib/physics/world_fwd.hpp"
#include "../../fruitlib/physics/group/object.hpp"
#include <sge/image2d/multi_loader_fwd.hpp>
#include <sge/parse/json/array_fwd.hpp>
#include <sge/renderer/device_fwd.hpp>
#include <sge/renderer/matrix4.hpp>
#include <sge/renderer/vertex_declaration_ptr.hpp>
#include <sge/shader/object.hpp>
#include <sge/camera/object_fwd.hpp>
#include <sge/time/callback.hpp>
#include <sge/time/duration.hpp>
#include <fcppt/chrono/duration.hpp>
#include <fcppt/math/matrix/matrix.hpp>
#include <fcppt/signal/object.hpp>
#include <fcppt/signal/auto_connection.hpp>
#include <fcppt/noncopyable.hpp>
#include <boost/mpl/vector/vector10.hpp>

namespace fruitapp
{
namespace fruit
{
/**
	The fruit manager is supposed to provide read-only access to the
	fruits and fruit prototypes. All modifying actions like "cut fruit"
	and "add new fruit" and "remove fruit" ultimately land here. In the
	future, this class might also "tag" fruits and track "double cuts".
 */
class manager
:
	public fruitlib::scenic::node<manager>
{
FCPPT_NONCOPYABLE(
	manager);
public:
	typedef
	boost::mpl::vector1<fruitlib::scenic::events::update>
	scene_reactions;

	explicit
	manager(
		fruitlib::scenic::optional_parent const &,
		fruit::prototype_sequence const &,
		sge::renderer::device &renderer,
		fruitlib::physics::world &,
		sge::camera::object &);

	// cut_fruit gets a duration indicating how long the new fruits are
	// to be banned from further cutting (which would result in paper
	// thin fruit slices)
	void
	cut(
		object const &,
		plane const &,
		fruitlib::physics::vector3 const &,
		sge::time::duration const &,
		sge::time::callback const &);

	void
	spawn(
		prototype const &proto,
		fruitlib::physics::scalar const mass,
		fruitlib::physics::vector3 const &position,
		fruitlib::physics::vector3 const &linear_velocity,
		fruitlib::physics::vector3 const &angular_velocity);

	object_sequence const &
	fruits() const;

	prototype_sequence const &
	prototypes() const;

	fcppt::signal::auto_connection
	cut_callback(
		fruitapp::fruit::callbacks::cut const &);

	fcppt::signal::auto_connection
	remove_callback(
		fruitapp::fruit::callbacks::remove const &);

	fcppt::signal::auto_connection
	spawn_callback(
		fruitapp::fruit::callbacks::spawn const &);

	fruitlib::physics::group::object &
	fruit_group();

	fruitlib::physics::group::object const &
	fruit_group() const;

	sge::renderer::vertex_declaration &
	vertex_declaration();

	sge::renderer::vertex_declaration const &
	vertex_declaration() const;

	~manager();

	void
	react(
		fruitlib::scenic::events::update const &);
private:
	prototype_sequence const &prototypes_;
	sge::renderer::device &renderer_;
	sge::camera::object &camera_;
	sge::renderer::vertex_declaration_ptr vertex_declaration_;
	fruitlib::physics::world &physics_world_;
	fruitlib::physics::group::object fruit_group_;
	object_sequence fruits_; 
	fcppt::signal::object<callbacks::cut_fn> cut_signal_;
	fcppt::signal::object<callbacks::remove_fn> remove_signal_;
	fcppt::signal::object<callbacks::spawn_fn> spawn_signal_;

	void
	delete_distant_fruits();
};
}
}

#endif
