#include "ingame.hpp"
#include "../media_path.hpp"
#include "../json/find_member.hpp"
#include "../events/toggle_mode.hpp"
#include "../events/render.hpp"
#include "../cut_mesh.hpp"
#include "../plane.hpp"
#include "../media_path.hpp"
#include "../model/vf/format.hpp"
#include "../model_to_mesh.hpp"
#include "../mesh_to_vertex_buffer.hpp"
#include "../math/plane/normalize.hpp"
#include <sge/camera/parameters.hpp>
#include <sge/camera/projection/perspective.hpp>
#include <sge/font/system.hpp>
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
#include <sge/image/multi_loader.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/texture.hpp>
#include <sge/renderer/aspect.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/renderer/vector3.hpp>
#include <sge/input/keyboard/collector.hpp>
#include <sge/image/file.hpp>
#include <sge/input/keyboard/key_event.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/shader/vf_to_string.hpp>
#include <sge/shader/scoped.hpp>
#include <sge/shader/mat4.hpp>
#include <sge/shader/variable.hpp>
#include <sge/shader/variable_type.hpp>
#include <sge/renderer/state/trampoline.hpp>
#include <sge/renderer/state/draw_mode.hpp>
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
#include <fcppt/math/vector/cross.hpp>
#include <fcppt/math/vector/dot.hpp>
#include <fcppt/math/vector/normalize.hpp>
#include <functional>

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
}

fruitcut::states::ingame::ingame(
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
          context<machine>().systems().renderer()->screen_size().w(),
          static_cast<sge::console::sprite_object::unit>(
            context<machine>().systems().renderer()->screen_size().h() / 2)))
      .elements()),
		static_cast<sge::console::output_line_limit>(
			1000)),
	console_connection_(
		context<machine>().systems().keyboard_collector()->key_callback(
			std::bind(
				&ingame::console_callback,
				this,
				std::placeholders::_1))),
	state_change_connection_(
		console_.insert(
			FCPPT_TEXT("tm"),
			std::bind(
				&ingame::toggle_mode,
				this,
				std::placeholders::_1,
				std::placeholders::_2),
			FCPPT_TEXT("Toggles between freelook mode and \"cut\" mode"))),
	shader_(
		context<machine>().systems().renderer(),
		media_path()/FCPPT_TEXT("shaders")/FCPPT_TEXT("vertex.glsl"),
		media_path()/FCPPT_TEXT("shaders")/FCPPT_TEXT("fragment.glsl"),
		sge::shader::vf_to_string<model::vf::format>(),
		{
			sge::shader::variable(
				"mvp",
				sge::shader::variable_type::uniform,
				sge::shader::mat4())
		},
		{}),
	model_(
		context<machine>().systems().md3_loader()->load(
			media_path()/
			FCPPT_TEXT("models")/
			json::find_member<fcppt::string>(
				context<machine>().config_file(),
				FCPPT_TEXT("test-model")))),
	mesh_(
		model_to_mesh(model_)),
	mesh_vb_(
		mesh_to_vertex_buffer(
			context<machine>().systems().renderer(),
			shader_,
			mesh_)),
	scoped_state_(
		context<machine>().systems().renderer(),
		sge::renderer::state::list
			(sge::renderer::state::bool_::clear_backbuffer = true))
{
	input_manager_.current_state(
		input_states_[
			current_input_state_]);
}

// This is called by the child states in the constructors. It changes
// the current input state, a variable which holds the information
// which state is active when the console isn't active.
void
fruitcut::states::ingame::current_input_state(
	input_states::type const _current_input_state)
{
	current_input_state_ =
		_current_input_state;

	if (!console_gfx_.active())
		input_manager_.current_state(
			input_states_[current_input_state_]);
}

boost::statechart::result
fruitcut::states::ingame::react(
	events::render const &)
{
	sge::shader::scoped scoped_shader(
		shader_);

	context<machine>().systems().renderer()->state(
		sge::renderer::state::list
			(sge::renderer::state::draw_mode::line));

	shader_.set_uniform(
		"mvp",
		camera_.mvp());

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
fruitcut::states::ingame::react(
	events::render_overlay const &)
{
	if (console_gfx_.active())
		console_gfx_.draw();

	return discard_event();
}

void
fruitcut::states::ingame::cut(
	sge::renderer::vector3 const &position,
	sge::renderer::vector3 const &direction1,
	sge::renderer::vector3 const &direction2)
{
	sge::renderer::vector3 const plane_normal = 
		fcppt::math::vector::normalize(
			fcppt::math::vector::cross(
				direction1,
				direction2));

	plane const p(
		plane_normal,
		fcppt::math::vector::dot(
			position,
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
fruitcut::states::ingame::camera()
{
	return camera_;
}

void
fruitcut::states::ingame::console_callback(
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
fruitcut::states::ingame::toggle_mode(
	sge::console::arg_list const &,
	sge::console::object &)
{
	post_event(
		events::toggle_mode());
}
