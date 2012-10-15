#include <fruitapp/projection_manager/object.hpp>
#include <fruitapp/viewport/manager.hpp>
#include <fruitlib/perspective_projection_information.hpp>
#include <fruitlib/scenic/base.hpp>
#include <sge/camera/has_mutable_projection.hpp>
#include <sge/parse/json/find_and_convert_member.hpp>
#include <sge/renderer/aspect.hpp>
#include <sge/renderer/projection/perspective_af.hpp>
#include <sge/viewport/manager.hpp>
#include <fcppt/move.hpp>
#include <fcppt/math/deg_to_rad.hpp>
#include <fcppt/signal/connection.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/tr1/functional.hpp>


fruitapp::projection_manager::object::object(
	sge::parse::json::object const &_projection_information,
	fruitapp::viewport::manager &_viewport_manager,
	sge::camera::has_mutable_projection &_camera)
:
	projection_change_signal_(),
	camera_(
		_camera),
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
	aspect_(),
	viewport_connection_(
		_viewport_manager.change_callback(
			std::tr1::bind(
				&object::viewport_callback,
				this,
				std::tr1::placeholders::_1),
			fruitapp::viewport::trigger_early(
				true)))
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
	projection_manager::projection_change_callback const &_projection_change_callback,
	projection_manager::trigger_early const &_trigger_early)
{
	fcppt::signal::auto_connection result(
		projection_change_signal_.connect(
			_projection_change_callback));

	if(_trigger_early.get() && aspect_)
		projection_change_signal_(
			*this->perspective_projection_information());

	return
		fcppt::move(
			result);
}

fruitapp::projection_manager::object::~object()
{
}

void
fruitapp::projection_manager::object::viewport_callback(
	sge::renderer::target::viewport const &_viewport)
{
	aspect_ =
		optional_aspect(
			sge::renderer::projection::aspect(
				sge::renderer::aspect(
					fcppt::math::dim::structure_cast<sge::renderer::screen_size>(
						_viewport.get().size()))));

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
