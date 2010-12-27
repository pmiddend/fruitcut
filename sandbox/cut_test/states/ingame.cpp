#include "ingame.hpp"
#include "../../../media_path.hpp"
#include "../../../json/find_member.hpp"
#include "../../../cut_mesh.hpp"
#include "../../../plane.hpp"
#include "../../../media_path.hpp"
#include "../../../model/vf/format.hpp"
#include "../../../model_to_mesh.hpp"
#include "../../../mesh_to_vertex_buffer.hpp"
#include "../../../math/plane/normalize.hpp"
#include "../events/toggle_mode.hpp"
#include "../events/render.hpp"
#include <sge/camera/parameters.hpp>
#include <sge/camera/projection/perspective.hpp>
#include <sge/font/system.hpp>
#include <sge/font/text/lit.hpp>
#include <sge/image/colors.hpp>
#include <sge/font/size_type.hpp>
#include <sge/console/sprite_object.hpp>
#include <sge/console/output_line_limit.hpp>
#include <sge/sprite/parameters_impl.hpp>
#include <sge/sprite/object.hpp>
#include <sge/console/sprite_parameters.hpp>
#include <sge/renderer/filter/linear.hpp>
#include <sge/texture/part_ptr.hpp>
#include <sge/texture/part_raw.hpp>
#include <sge/image2d/multi_loader.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/texture.hpp>
#include <sge/renderer/aspect.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/renderer/vector3.hpp>
#include <sge/input/keyboard/collector.hpp>
#include <sge/image2d/file.hpp>
#include <sge/image2d/create_texture.hpp>
#include <sge/input/keyboard/key_event.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/shader/vf_to_string.hpp>
#include <sge/shader/scoped.hpp>
#include <sge/shader/mat4.hpp>
#include <sge/shader/variable.hpp>
#include <sge/shader/variable_type.hpp>
#include <sge/renderer/state/trampoline.hpp>
#include <sge/renderer/state/draw_mode.hpp>
#include <sge/renderer/state/depth_func.hpp>
#include <sge/renderer/state/float.hpp>
#include <sge/renderer/state/scoped.hpp>
#include <sge/renderer/state/list.hpp>
#include <sge/renderer/state/bool.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/scoped_vertex_buffer.hpp>
#include <sge/renderer/vertex_buffer.hpp>
#include <sge/model/loader.hpp>
#include <fcppt/text.hpp>
#include <fcppt/string.hpp>
#include <fcppt/math/deg_to_rad.hpp>
#include <fcppt/math/vector/arithmetic.hpp>
#include <fcppt/math/vector/cross.hpp>
#include <fcppt/math/vector/dot.hpp>
#include <fcppt/math/vector/normalize.hpp>
#include <fcppt/math/vector/construct.hpp>
#include <fcppt/math/vector/narrow_cast.hpp>
#include <fcppt/math/matrix/translation.hpp>
#include <fcppt/math/matrix/arithmetic.hpp>
#include <fcppt/math/matrix/rotation_axis.hpp>
#include <fcppt/math/matrix/vector.hpp>
#include <fcppt/math/matrix/transpose.hpp>
#include <fcppt/assign/make_container.hpp>
#include <boost/bind.hpp>

namespace
{
template<typename PtrArray>
std::auto_ptr<PtrArray>
make_state_vector(
	fruitcut::input::state_manager &stm)
{
	std::auto_ptr<PtrArray>	pa(
		new PtrArray());
	pa->replace(
		fruitcut::input_states::console,
		new fruitcut::input::state(
			stm));
	pa->replace(
		fruitcut::input_states::cut,
		new fruitcut::input::state(
			stm));
	pa->replace(
		fruitcut::input_states::freelook,
		new fruitcut::input::state(
			stm));
	return pa;
}

// NOTE: Could this find its way to fcppt?
template<typename T>
typename
fcppt::math::vector::static_<T,3>::type const
multiply_matrix4_vector3(
	typename
	fcppt::math::matrix::static_<T,4,4>::type const &m,
	typename
	fcppt::math::vector::static_<T,3>::type const &v)
{
	return 
		fcppt::math::vector::narrow_cast
		<
			typename fcppt::math::vector::static_<T,3>::type
		>(
			m * 
			fcppt::math::vector::construct(
				v,
				static_cast<T>(0)));
}
}

fruitcut::sandbox::cut_test::states::ingame::ingame(
	my_context const my_ctx)
:
	my_base(
		my_ctx),
	input_manager_(
		context<machine>().systems()),
	input_states_(
		make_state_vector<input_state_map>(
			input_manager_)),
	current_input_state_(
		input_states::freelook),
	camera_(
		sge::camera::parameters(
			//graphics::camera::projection::orthogonal()
			sge::camera::projection::perspective(
				sge::renderer::aspect<sge::renderer::scalar>(
					context<machine>().systems().renderer()->screen_size()),
				fcppt::math::deg_to_rad(
					fruitcut::json::find_member<sge::renderer::scalar>(
						context<machine>().config_file(),
						FCPPT_TEXT("graphics/camera/fov"))),
				fruitcut::json::find_member<sge::renderer::scalar>(
					context<machine>().config_file(),
					FCPPT_TEXT("graphics/camera/near")),
				fruitcut::json::find_member<sge::renderer::scalar>(
					context<machine>().config_file(),
					FCPPT_TEXT("graphics/camera/far"))),
			fruitcut::json::find_member<sge::renderer::scalar>(
				context<machine>().config_file(),
				FCPPT_TEXT("graphics/camera/movement-speed")),
			fruitcut::json::find_member<sge::renderer::scalar>(
				context<machine>().config_file(),
				FCPPT_TEXT("graphics/camera/rotation-speed")),
			sge::renderer::vector3::null(),
			input_states_[input_states::freelook],
			input_states_[input_states::freelook])),
	console_(
		FCPPT_TEXT('/')),
	console_gfx_(
		console_,
		context<machine>().systems().renderer(),
		sge::image::colors::black(),
		context<machine>().systems().font_system()->create_font(
			media_path()
			/ FCPPT_TEXT("fonts")
			/ FCPPT_TEXT("default.ttf"),
			static_cast<sge::font::size_type>(
				15)),
		context<machine>().systems().keyboard_collector(),
		sge::console::sprite_object(
      sge::console::sprite_parameters()
      .texture(
				sge::texture::part_ptr(
					new sge::texture::part_raw(
						context<machine>().systems().renderer()->create_texture(
							context<machine>().systems().image_loader().load(
								media_path() 
									/ FCPPT_TEXT("textures") 
									/ FCPPT_TEXT("console_back.png"))->view(),
							sge::renderer::filter::linear,
							sge::renderer::resource_flags::none))))
      .pos(
        sge::console::sprite_object::point::null())
      .size(
        sge::console::sprite_object::dim(
					static_cast<sge::console::sprite_object::unit>(
						context<machine>().systems().renderer()->screen_size().w()),
          static_cast<sge::console::sprite_object::unit>(
            context<machine>().systems().renderer()->screen_size().h() / 2)))
      .elements()),
		static_cast<sge::console::output_line_limit>(
			1000)),
	console_connection_(
		// FIXME: phoenix doesn't play along with fcppt::function, so boost::bind here
		context<machine>().systems().keyboard_collector()->key_callback(
			boost::bind(
				&ingame::console_callback,
				this,
				_1))),
	state_change_connection_(
		console_.insert(
			SGE_FONT_TEXT_LIT("tm"),
			boost::bind(
				&ingame::toggle_mode,
				this,
				_1,
				_2),
			SGE_FONT_TEXT_LIT("Toggles between freelook mode and \"cut\" mode"))),
	wireframe_(
		false),
	wireframe_connection_(
		console_.insert(
			SGE_FONT_TEXT_LIT("wireframe"),
			// This could, again, be a phoenix actor, but phoenix doesn't
			// like fcppt::function
			boost::bind(
				&ingame::toggle_wireframe,
				this,
				_1,
				_2),
			SGE_FONT_TEXT_LIT("Toggle wireframe on/off"))),
	model_(
		context<machine>().systems().md3_loader()->load(
			media_path()/
			FCPPT_TEXT("models")/
			json::find_member<fcppt::string>(
				context<machine>().config_file(),
				FCPPT_TEXT("test-model")))),
	model_texture_(
		sge::image2d::create_texture(
			media_path()/
			FCPPT_TEXT("textures")/
			json::find_member<fcppt::string>(
				context<machine>().config_file(),
				FCPPT_TEXT("test-model-texture")),
			context<machine>().systems().renderer(),
			context<machine>().systems().image_loader(),
			sge::renderer::filter::linear,
			sge::renderer::resource_flags::none)),
	shader_(
		context<machine>().systems().renderer(),
		media_path()/FCPPT_TEXT("shaders")/FCPPT_TEXT("vertex.glsl"),
		media_path()/FCPPT_TEXT("shaders")/FCPPT_TEXT("fragment.glsl"),
		sge::shader::vf_to_string<model::vf::format>(),
		fcppt::assign::make_container<sge::shader::variable_sequence>(
			sge::shader::variable(
					"mvp",
					sge::shader::variable_type::uniform,
					sge::shader::mat4())),
		fcppt::assign::make_container<sge::shader::sampler_sequence>(
			sge::shader::sampler(
				"texture",
				model_texture_))),
	mesh_(
		model_to_mesh(
			model_)),
	mesh_vb_(
		mesh_to_vertex_buffer(
			context<machine>().systems().renderer(),
			shader_,
			mesh_)),
	scoped_state_(
		context<machine>().systems().renderer(),
		sge::renderer::state::list
			(sge::renderer::state::bool_::clear_backbuffer = true)
			(sge::renderer::state::bool_::clear_zbuffer = true)
			(sge::renderer::state::depth_func::less)
			(sge::renderer::state::float_::zbuffer_clear_val = 1.0f)),
	mesh_translation_(
		sge::renderer::vector3(
			3,
			0,
			0)),
	mesh_rotation_(
		fcppt::math::matrix::rotation_axis(
			fcppt::math::deg_to_rad(sge::renderer::scalar(30)),
			sge::renderer::vector3(
				0,1,0)))
{
	input_manager_.current_state(
		input_states_[
			current_input_state_]);
}

// This is called by the child states in the constructors. It changes
// the current input state, a variable which holds the information
// which state is active when the console isn't active.
void
fruitcut::sandbox::cut_test::states::ingame::current_input_state(
	input_states::type const _current_input_state)
{
	current_input_state_ =
		_current_input_state;

	if (!console_gfx_.active())
		input_manager_.current_state(
			input_states_[current_input_state_]);
}

boost::statechart::result
fruitcut::sandbox::cut_test::states::ingame::react(
	events::render const &)
{
	sge::shader::scoped scoped_shader(
		shader_);

	shader_.set_uniform(
		"mvp",
		camera_.mvp() 
			* fcppt::math::matrix::translation(mesh_translation_)
			* mesh_rotation_);

	sge::renderer::scoped_vertex_buffer scoped_vb(
		context<machine>().systems().renderer(),
		mesh_vb_);

	context<machine>().systems().renderer()->render(
		sge::renderer::first_vertex(0),
		sge::renderer::vertex_count(
			mesh_vb_->size()),
		sge::renderer::nonindexed_primitive_type::triangle);

	return discard_event();
}

boost::statechart::result
fruitcut::sandbox::cut_test::states::ingame::react(
	events::render_overlay const &)
{
	if (console_gfx_.active())
		console_gfx_.draw();

	return discard_event();
}

void
fruitcut::sandbox::cut_test::states::ingame::cut(
	sge::renderer::vector3 const &position,
	sge::renderer::vector3 const &direction1,
	sge::renderer::vector3 const &direction2)
{
	// NOTE: For rotation matrices M and vectors a,b the following holds:
	// cross(M*a,M*b) = M*cross(a,b)
	sge::renderer::vector3 const plane_normal = 
		multiply_matrix4_vector3<sge::renderer::scalar>(
			fcppt::math::matrix::transpose(
				mesh_rotation_),
			fcppt::math::vector::cross(
				direction1,
				direction2));

	plane const p(
		plane_normal,
		fcppt::math::vector::dot(
			multiply_matrix4_vector3<sge::renderer::scalar>(
				fcppt::math::matrix::transpose(
					mesh_rotation_),
				position - mesh_translation_),
			plane_normal));

	mesh_ = 
		cut_mesh(
			mesh_,
			p);
	mesh_vb_ = 
		mesh_to_vertex_buffer(
			context<machine>().systems().renderer(),
			shader_,
			mesh_);
}

sge::camera::object &
fruitcut::sandbox::cut_test::states::ingame::camera()
{
	return camera_;
}

void
fruitcut::sandbox::cut_test::states::ingame::console_callback(
	sge::input::keyboard::key_event const &e)
{
	if (!e.pressed())
		return;

	switch (e.key().code())
	{
		case sge::input::keyboard::key_code::f1:
			console_gfx_.active(
				!console_gfx_.active());
			input_manager_.current_state(
				input_states_[
					console_gfx_.active() 
					? 
						input_states::console
					:
						current_input_state_]);
			break;
		default:
			break;
	}
}

void
fruitcut::sandbox::cut_test::states::ingame::toggle_mode(
	sge::console::arg_list const &,
	sge::console::object &)
{
	post_event(
		events::toggle_mode());
}

void
fruitcut::sandbox::cut_test::states::ingame::toggle_wireframe(
	sge::console::arg_list const &,
	sge::console::object &)
{
	context<machine>().systems().renderer()->state(
		sge::renderer::state::list(
			wireframe_
			? 
				sge::renderer::state::draw_mode::fill 
			: sge::renderer::state::draw_mode::line));
	wireframe_ = !wireframe_;
}