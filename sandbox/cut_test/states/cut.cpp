#include "cut.hpp"
#include "freelook.hpp"
#include "../../../media_path.hpp"
#include "../../../sprite/parameters.hpp"
#include "../../../sprite/object.hpp"
#include "../../../dim2.hpp"
#include <sge/sprite/external_system_impl.hpp>
#include <sge/sprite/render_one.hpp>
#include <sge/sprite/parameters.hpp>
#include <sge/sprite/default_equal.hpp>
#include <sge/image2d/multi_loader.hpp>
#include <sge/renderer/filter/linear.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/renderer/vector2.hpp>
#include <sge/renderer/vector3.hpp>
#include <sge/renderer/matrix4.hpp>
#include <sge/renderer/device.hpp>
#include <sge/texture/part_raw.hpp>
#include <sge/texture/part_ptr.hpp>
#include <sge/input/mouse/collector.hpp>
#include <sge/input/mouse/axis.hpp>
#include <sge/input/mouse/axis_event.hpp>
#include <sge/input/mouse/button_event.hpp>
#include <sge/input/mouse/button_code.hpp>
#include <sge/image2d/file.hpp>
#include <sge/image/color/init.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/math/vector/dim.hpp>
#include <fcppt/math/dim/arithmetic.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/math/matrix/inverse.hpp>
#include <fcppt/math/matrix/arithmetic.hpp>
#include <fcppt/math/vector/output.hpp>
#include <fcppt/io/cout.hpp>

#include <fcppt/math/vector/static.hpp>
#include <fcppt/math/dim/static.hpp>
#include <fcppt/math/matrix/static.hpp>
#include <fcppt/math/dim/basic_impl.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/math/matrix/basic_impl.hpp>

#include <boost/bind.hpp>

namespace
{
template<typename T>
typename 
fcppt::math::vector::static_<T,3>::type const
unproject(
	typename fcppt::math::vector::static_<T,3>::type const &window_coordinates,
	typename fcppt::math::matrix::static_<T,4,4>::type const &inverse_mvp,
	typename fcppt::math::vector::static_<T,2>::type const &viewport_pos,
	typename fcppt::math::dim::static_<T,2>::type const &viewport_dim)
{
	/*
	return 
		fcppt::math::vector::narrow_cast
		<
			typename fcppt::math::vector::static_<T,3>::type
		>(
			inverse_mvp * 
			typename fcppt::math::vector::static_<T,4>::type(
				2 * (window_coordinates.x() - viewport_pos.x()) / viewport_dim.w() - 1,
				2 * (window_coordinates.y() - viewport_pos.y()) / viewport_dim.h() - 1,
				2 * window_coordinates.z(),
				1));
	*/

	// Copied from http://www.opengl.org/wiki/GluProject_and_gluUnProject_code
	typename fcppt::math::vector::static_<T,4>::type result = 
		inverse_mvp * 
		typename fcppt::math::vector::static_<T,4>::type(
			2 * (window_coordinates.x() - viewport_pos.x()) / viewport_dim.w() - 1,
			2 * (window_coordinates.y() - viewport_pos.y()) / viewport_dim.h() - 1,
			2 * window_coordinates.z(),
			1);
	
	result[3] = 1/result[3];
	return 
		fcppt::math::vector::narrow_cast
		<
			typename fcppt::math::vector::static_<T,3>::type
		>(
			result[3] * result);
}
}

fruitcut::sandbox::cut_test::states::cut::cut(
	my_context ctx)
:
	my_base(
		ctx),
	ss_(
		context<machine>().systems().renderer()),
	cursor_(
		sprite::parameters()
			.texture_size()
			// default value is false (or undefined - both wrong)
			.visible(
				true)
			.color(
				sprite::object::color_type(
					(sge::image::color::init::red %= 1.0)
					(sge::image::color::init::green %= 1.0)
					(sge::image::color::init::blue %= 1.0)
					(sge::image::color::init::alpha %= 1.0)))
			.system(
				&ss_)
			.texture(
				sge::texture::part_ptr(
					new sge::texture::part_raw(
						context<machine>().systems().renderer()->create_texture(
							context<machine>().systems().image_loader().load(
								media_path() 
									/ FCPPT_TEXT("textures") 
									/ FCPPT_TEXT("cursor.png"))->view(),
							sge::renderer::filter::linear,
							sge::renderer::resource_flags::none))))
			.pos(
				sprite::object::point(100,100))
			.elements()),
	mouse_axis_connection_(
		context<machine>().systems().mouse_collector()->axis_callback(
			boost::bind(
				&cut::mouse_axis_callback,
				this,
				_1))),
	mouse_button_connection_(
		context<machine>().systems().mouse_collector()->button_callback(
			boost::bind(
				&cut::mouse_button_callback,
				this,
				_1))),
	point1_(
		sprite::parameters()
			.texture_size()
			.visible(
				false)
			.color(
				sprite::object::color_type(
					(sge::image::color::init::red %= 1.0)
					(sge::image::color::init::green %= 1.0)
					(sge::image::color::init::blue %= 1.0)
					(sge::image::color::init::alpha %= 1.0)))
			.system(
				&ss_)
			.texture(
				sge::texture::part_ptr(
					new sge::texture::part_raw(
						context<machine>().systems().renderer()->create_texture(
							context<machine>().systems().image_loader().load(
								media_path() 
									/ FCPPT_TEXT("textures") 
									/ FCPPT_TEXT("marker.png"))->view(),
							sge::renderer::filter::linear,
							sge::renderer::resource_flags::none))))
			.elements()),
	point2_(
		point1_)
{
	context<ingame>().current_input_state(
		input_states::cut);
}

boost::statechart::result
fruitcut::sandbox::cut_test::states::cut::react(
	events::render_overlay const &)
{
	ss_.render_all(
		sge::sprite::default_equal());
	return forward_event();
}

boost::statechart::result
fruitcut::sandbox::cut_test::states::cut::react(
	events::toggle_mode const &)
{
	return transit<freelook>();
}

void
fruitcut::sandbox::cut_test::states::cut::mouse_axis_callback(
	sge::input::mouse::axis_event const &e)
{
	// Yeah, no switch for axis_type here. TODO!
	cursor_.pos( 
		cursor_.pos() + 
		sprite::object::point(
			static_cast<sprite::object::point::value_type>(
				e.axis() == sge::input::mouse::axis::x
				?
					e.axis_value()
				:
					0),
			static_cast<sprite::object::point::value_type>(
				e.axis() == sge::input::mouse::axis::y
				?
					e.axis_value()
				:
					0)));
}

void
fruitcut::sandbox::cut_test::states::cut::mouse_button_callback(
	sge::input::mouse::button_event const &e)
{
	if (!e.pressed())
		return;

	switch (e.button_code())
	{
		case sge::input::mouse::button_code::left:
			if (!point1_.visible())
			{
				point1_.pos(
					cursor_.pos() - point1_.size()/2);
				point1_.visible(
					true);
			}
			else if (!point2_.visible())
			{
				point2_.pos(
					cursor_.pos() - point2_.size()/2);
				point2_.visible(
					true);

				// Just for debugging purposes
				dim2 const ss = 
					fcppt::math::dim::structure_cast<dim2>(
						context<machine>().systems().renderer()->screen_size());

				sge::renderer::matrix4 const inverse_mvp =
					fcppt::math::matrix::inverse(
						context<ingame>().camera().mvp());
				sge::renderer::vector3 const 
					point1_projected = 
						unproject<sge::renderer::scalar>(
							sge::renderer::vector3(
								static_cast<sge::renderer::scalar>(
									point1_.x() + point1_.w()/2),
								// NOTE: We have to switch the viewport here, taken from:
								// http://nehe.gamedev.net/data/articles/article.asp?article=13
								// Now Windows coordinates start with (0, 0) being at
								// the top left whereas OpenGL coords start at the
								// lower left. To convert to OpenGL coordinates we do
								// the following:
								ss.h() - static_cast<sge::renderer::scalar>(
									point1_.y() + point1_.h()/2),
								0),
							inverse_mvp,
							sge::renderer::vector2::null(),
							fcppt::math::dim::structure_cast<dim2>(
								context<machine>().systems().renderer()->screen_size())),
					point2_projected = 
						unproject<sge::renderer::scalar>(
							sge::renderer::vector3(
								static_cast<sge::renderer::scalar>(
									point2_.x() + point2_.w()/2),
								// See above
								ss.h() - static_cast<sge::renderer::scalar>(
									point2_.y() + point2_.h()/2),
								0),
							inverse_mvp,
							sge::renderer::vector2::null(),
							fcppt::math::dim::structure_cast<dim2>(
								context<machine>().systems().renderer()->screen_size())),
					point3_projected = 
						unproject<sge::renderer::scalar>(
							sge::renderer::vector3(
								static_cast<sge::renderer::scalar>(
									point1_.x() + point1_.w()/2),
								// See above
								ss.h() - static_cast<sge::renderer::scalar>(
									point1_.y() + point1_.h()/2),
								0.5),
							inverse_mvp,
							sge::renderer::vector2::null(),
							fcppt::math::dim::structure_cast<dim2>(
								context<machine>().systems().renderer()->screen_size())),
					first_plane_vector = 
						point2_projected - point1_projected,
					second_plane_vector = 
						point3_projected - point1_projected;

				context<ingame>().cut(
					point1_projected,
					first_plane_vector,
					second_plane_vector);
			}
			break;
		default:
			break;
	}
}