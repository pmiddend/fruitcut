#include <fruitapp/projection_manager/object.hpp>
#include <fruitapp/viewport/change_callback.hpp>
#include <fruitapp/viewport/manager.hpp>
#include <fruitlib/perspective_projection_information.hpp>
#include <fruitlib/scenic/base.hpp>
#include <sge/camera/has_mutable_projection.hpp>
#include <sge/camera/coordinate_system/object.hpp>
#include <sge/camera/matrix_conversion/world_projection.hpp>
#include <sge/parse/json/find_and_convert_member.hpp>
#include <sge/renderer/aspect.hpp>
#include <sge/renderer/vector4.hpp>
#include <sge/renderer/projection/perspective_af.hpp>
#include <sge/renderer/target/base.hpp>
#include <sge/viewport/manager.hpp>
#include <fcppt/cast/int_to_float_fun.hpp>
#include <fcppt/cast/size_fun.hpp>
#include <fcppt/math/deg_to_rad.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/math/dim/to_unsigned.hpp>
#include <fcppt/math/dim/to_vector.hpp>
#include <fcppt/math/matrix/vector.hpp>
#include <fcppt/math/vector/arithmetic.hpp>
#include <fcppt/math/vector/construct.hpp>
#include <fcppt/math/vector/structure_cast.hpp>
#include <fcppt/optional/map.hpp>
#include <fcppt/signal/connection.hpp>
#include <fcppt/config/external_begin.hpp>
#include <functional>
#include <fcppt/config/external_end.hpp>


fruitapp::projection_manager::object::object(
	sge::parse::json::object const &_projection_information,
	fruitapp::viewport::manager &_viewport_manager,
	sge::camera::has_mutable_projection &_camera)
:
	target_(
		_viewport_manager.target()),
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
			fruitapp::viewport::change_callback{
				std::bind(
					&object::viewport_callback,
					this,
					std::placeholders::_1
				)
			},
			fruitapp::viewport::trigger_early(
				true)))
{
}

fruitlib::optional_perspective_projection_information const
fruitapp::projection_manager::object::perspective_projection_information() const
{
	return
		fcppt::optional::map(
			aspect_,
			[
				this
			](
				sge::renderer::projection::aspect const _aspect
			)
			{
				return
					fruitlib::perspective_projection_information(
						fov_,
						near_,
						far_,
						_aspect
					);
			}
		);
}

fcppt::signal::auto_connection
fruitapp::projection_manager::object::projection_change_callback(
	projection_manager::projection_change_callback const &_projection_change_callback,
	projection_manager::trigger_early const &_trigger_early)
{
	fcppt::signal::auto_connection result(
		projection_change_signal_.connect(
			_projection_change_callback));

	if(_trigger_early.get() && aspect_.has_value())
		projection_change_signal_(
			this->perspective_projection_information().get_unsafe()); // TODO

	return
		result;
}

sge::renderer::vector2 const
fruitapp::projection_manager::object::project_point(
	sge::renderer::vector3 const &_point) const
{
	sge::renderer::pixel_rect const viewport_rect(
		target_.viewport().get());

	sge::renderer::vector4 const result(
		sge::camera::matrix_conversion::world_projection(
			camera_.coordinate_system(),
			camera_.projection_matrix()) *
		fcppt::math::vector::construct(
			_point,
			static_cast<sge::renderer::scalar>(
				1.0f)));

	sge::renderer::vector2 const
		result_2d(
			result.x() / result.w(),
			result.y() / result.w()),
		result_2d_noninverted(
			(result_2d + sge::renderer::vector2(1.0f,1.0f)) /
			sge::renderer::vector2(2.0f,2.0f));

	return
		sge::renderer::vector2(
			result_2d_noninverted.x(),
			1.0f - result_2d_noninverted.y()
		)
		*
		fcppt::math::vector::structure_cast<
			sge::renderer::vector2,
			fcppt::cast::int_to_float_fun
		>(
			fcppt::math::dim::to_vector(
				viewport_rect.size()
			)
		);
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
					fcppt::math::dim::structure_cast<sge::renderer::screen_size, fcppt::cast::size_fun>(
						fcppt::math::dim::to_unsigned(
							_viewport.get().size())))));

	camera_.update_projection_matrix(
		sge::camera::projection_matrix(
			sge::renderer::projection::perspective_af(
				aspect_.get_unsafe(), // TODO
				fov_,
				near_,
				far_)));

	projection_change_signal_(
		fruitlib::perspective_projection_information(
			fov_,
			near_,
			far_,
			aspect_.get_unsafe())); // TODO
}
