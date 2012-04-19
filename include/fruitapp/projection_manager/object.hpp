#ifndef FRUITAPP_PROJECTION_MANAGER_OBJECT_HPP_INCLUDED
#define FRUITAPP_PROJECTION_MANAGER_OBJECT_HPP_INCLUDED

#include <fruitapp/machine_fwd.hpp>
#include <fruitlib/scenic/base_fwd.hpp>
#include <sge/camera/has_mutable_projection_fwd.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <sge/renderer/device_fwd.hpp>
#include <sge/renderer/projection/far.hpp>
#include <sge/renderer/projection/fov.hpp>
#include <sge/renderer/projection/near.hpp>
#include <sge/viewport/manager_fwd.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/signal/scoped_connection.hpp>


namespace fruitapp
{
namespace projection_manager
{
class object
{
FCPPT_NONCOPYABLE(
	object);
public:
	explicit
	object(
		sge::parse::json::object const &,
		fruitlib::scenic::base &,
		sge::viewport::manager &,
		sge::renderer::device &,
		sge::camera::has_mutable_projection &);

	~object();
private:
	fruitlib::scenic::base &root_node_;
	sge::renderer::device &renderer_;
	sge::camera::has_mutable_projection &camera_;
	fcppt::signal::scoped_connection viewport_connection_;
	sge::renderer::projection::near const near_;
	sge::renderer::projection::far const far_;
	sge::renderer::projection::fov const fov_;

	void
	viewport_callback();
};
}
}

#endif

