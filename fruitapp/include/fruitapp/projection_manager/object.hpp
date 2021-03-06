#ifndef FRUITAPP_PROJECTION_MANAGER_OBJECT_HPP_INCLUDED
#define FRUITAPP_PROJECTION_MANAGER_OBJECT_HPP_INCLUDED

#include <fruitapp/machine_fwd.hpp>
#include <fruitapp/projection_manager/projection_change_callback.hpp>
#include <fruitapp/projection_manager/projection_change_callback_fn.hpp>
#include <fruitapp/projection_manager/trigger_early.hpp>
#include <fruitapp/viewport/manager_fwd.hpp>
#include <fruitlib/optional_perspective_projection_information.hpp>
#include <sge/camera/has_mutable_projection_fwd.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <sge/renderer/aspect.hpp>
#include <sge/renderer/vector2.hpp>
#include <sge/renderer/vector3.hpp>
#include <sge/renderer/projection/aspect.hpp>
#include <sge/renderer/projection/far.hpp>
#include <sge/renderer/projection/fov.hpp>
#include <sge/renderer/projection/near.hpp>
#include <sge/renderer/target/base_fwd.hpp>
#include <sge/renderer/target/viewport.hpp>
#include <sge/viewport/manager_fwd.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/optional/object.hpp>
#include <fcppt/signal/auto_connection.hpp>
#include <fcppt/signal/object.hpp>


namespace fruitapp
{
namespace projection_manager
{
class object
{
FCPPT_NONCOPYABLE(
	object);
public:
	object(
		sge::parse::json::object const &,
		fruitapp::viewport::manager &,
		sge::camera::has_mutable_projection &);

	fruitlib::optional_perspective_projection_information const
	perspective_projection_information() const;

	fcppt::signal::auto_connection
	projection_change_callback(
		projection_manager::projection_change_callback const &,
		projection_manager::trigger_early const &);

	sge::renderer::vector2 const
	project_point(
		sge::renderer::vector3 const &) const;

	~object();
private:
	typedef
	fcppt::optional::object<sge::renderer::projection::aspect>
	optional_aspect;

	sge::renderer::target::base const &target_;
	fcppt::signal::object<projection_manager::projection_change_callback_fn> projection_change_signal_;
	sge::camera::has_mutable_projection &camera_;
	sge::renderer::projection::near const near_;
	sge::renderer::projection::far const far_;
	sge::renderer::projection::fov const fov_;
	optional_aspect aspect_;
	fcppt::signal::auto_connection viewport_connection_;

	void
	viewport_callback(
		sge::renderer::target::viewport const &);
};
}
}

#endif

