#ifndef FRUITCUT_APP_FRUIT_MANAGER_HPP_INCLUDED
#define FRUITCUT_APP_FRUIT_MANAGER_HPP_INCLUDED

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
#include "../../scenic/nodes/intrusive.hpp"
#include "../../physics/world_fwd.hpp"
#include <sge/image2d/multi_loader_fwd.hpp>
#include <sge/model/loader_fwd.hpp>
#include <sge/parse/json/array_fwd.hpp>
#include <sge/renderer/device_fwd.hpp>
#include <sge/renderer/vertex_declaration_ptr.hpp>
#include <sge/shader/object.hpp>
#include <sge/camera/object_fwd.hpp>
#include <sge/time/callback.hpp>
#include <sge/time/duration.hpp>
#include <fcppt/chrono/duration.hpp>
#include <fcppt/signal/object.hpp>
#include <fcppt/signal/auto_connection.hpp>
#include <fcppt/noncopyable.hpp>

namespace fruitcut
{
namespace app
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
	public scenic::nodes::intrusive
{
FCPPT_NONCOPYABLE(
	manager);
public:
	explicit
	manager(
		sge::parse::json::array const &v,
		sge::model::loader &model_loader,
		sge::image2d::multi_loader &image_loader,
		sge::renderer::device &renderer,
		physics::world &,
		sge::camera::object &);

	// cut_fruit gets a duration indicating how long the new fruits are
	// to be banned from further cutting (which would result in paper
	// thin fruit slices)
	void
	cut(
		object const &,
		plane const &,
		physics::vector3 const &,
		sge::time::duration const &,
		sge::time::callback const &);

	void
	spawn(
		prototype const &proto,
		physics::scalar const mass,
		physics::vector3 const &position,
		physics::vector3 const &linear_velocity,
		physics::vector3 const &angular_velocity);

	object_sequence const &
	fruits() const;

	prototype_sequence const &
	prototypes() const;

	fcppt::signal::auto_connection
	cut_callback(
		fruitcut::app::fruit::callbacks::cut const &);

	fcppt::signal::auto_connection
	remove_callback(
		fruitcut::app::fruit::callbacks::remove const &);

	fcppt::signal::auto_connection
	spawn_callback(
		fruitcut::app::fruit::callbacks::spawn const &);

	~manager();
private:
	sge::renderer::device &renderer_;
	sge::camera::object &camera_;
	sge::renderer::vertex_declaration_ptr vertex_declaration_;
	physics::world &physics_world_;
	prototype_sequence prototypes_;
	object_sequence fruits_; 
	sge::shader::object fruit_shader_;
	fcppt::signal::object<callbacks::cut_fn> cut_signal_;
	fcppt::signal::object<callbacks::remove_fn> remove_signal_;
	fcppt::signal::object<callbacks::spawn_fn> spawn_signal_;

	void
	delete_distant_fruits();

	void
	update();

	void
	render();
};
}
}
}

#endif