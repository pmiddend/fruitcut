#include <fruitapp/projection_manager/object.hpp>
#include <fruitlib/perspective_projection_information.hpp>
#include <fruitlib/scenic/base.hpp>
#include <sge/camera/has_mutable_projection.hpp>
#include <sge/parse/json/find_and_convert_member.hpp>
#include <sge/renderer/active_target.hpp>
#include <sge/renderer/aspect_from_viewport.hpp>
#include <sge/renderer/target_base.hpp>
#include <sge/renderer/projection/perspective_af.hpp>
#include <sge/viewport/manager.hpp>
#include <fcppt/math/deg_to_rad.hpp>
#include <fcppt/signal/connection.hpp>
#include <fcppt/tr1/functional.hpp>


fruitapp::projection_manager::object::object(
	sge::parse::json::object const &_projection_information,
	sge::viewport::manager &_viewport_manager,
	sge::renderer::device &_renderer,
	sge::camera::has_mutable_projection &_camera)
:
	projection_change_signal_(),
	renderer_(
		_renderer),
	camera_(
		_camera),
	viewport_connection_(
		_viewport_manager.manage_callback(
			std::tr1::bind(
				&object::viewport_callback,
				this))),
	near_(
		sge::parse::json::find_and_convert_member<sge::renderer::scalar>(
			_projection_information,
			sge::parse::json::path(
				FCPPT_TEXT("near")))),
	far_(
		sge::parse::json::find_and_convert_member<sge::renderer::scalar>(
			_projection_information,
			sge::parse::json::path(
				FCPPT_TEXT("far")))),
	fov_(
		fcppt::math::deg_to_rad(
			sge::parse::json::find_and_convert_member<sge::renderer::scalar>(
				_projection_information,
				sge::parse::json::path(
					FCPPT_TEXT("fov"))))),
	aspect_()
{
}

fruitlib::optional_perspective_projection_information const
fruitapp::projection_manager::object::perspective_projection_information() const
{
	return
		aspect_
		?
			fruitlib::optional_perspective_projection_information(
				fruitlib::perspective_projection_information(
					fov_,
					near_,
					far_,
					*aspect_))
		:
			fruitlib::optional_perspective_projection_information();
}

fcppt::signal::auto_connection
fruitapp::projection_manager::object::projection_change_callback(
	projection_manager::projection_change_callback const &_projection_change_callback)
{
	return
		projection_change_signal_.connect(
			_projection_change_callback);
}

fruitapp::projection_manager::object::~object()
{
}

void
fruitapp::projection_manager::object::viewport_callback()
{
	aspect_ =
		optional_aspect(
			sge::renderer::projection::aspect(
				sge::renderer::aspect_from_viewport(
					sge::renderer::active_target(
						renderer_).viewport())));

	camera_.update_projection_matrix(
		sge::camera::projection_matrix(
			sge::renderer::projection::perspective_af(
				*aspect_,
				fov_,
				near_,
				far_)));

	projection_change_signal_(
		fruitlib::perspective_projection_information(
			fov_,
			near_,
			far_,
			*aspect_));
}
