#include "ingame.hpp"
#include "running.hpp"
#include "../model_vf/format.hpp"
#include "../model_to_mesh.hpp"
#include "../../input/state.hpp"
#include "../../json/find_member.hpp"
#include "../../media_path.hpp"
#include "../events/toggle_pause.hpp"
#include "../../physics/vector3.hpp"
#include "../../physics/box.hpp"
#include "../../physics/matrix4.hpp"
#include "../../physics/scalar.hpp"
#include "../mesh.hpp"
#include "../box3.hpp"
#include "../cut_mesh.hpp"
#include <sge/model/loader.hpp>
#include <sge/systems/instance.hpp>
#include <sge/input/keyboard/action.hpp>
#include <sge/input/keyboard/key_code.hpp>
#include <sge/image2d/multi_loader.hpp>
#include <sge/image2d/file.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/scoped_vertex_buffer.hpp>
#include <sge/renderer/vertex_buffer.hpp>
#include <sge/renderer/first_vertex.hpp>
#include <sge/renderer/vertex_count.hpp>
#include <sge/renderer/nonindexed_primitive_type.hpp>
#include <sge/renderer/matrix4.hpp>
#include <sge/renderer/filter/linear.hpp>
#include <sge/renderer/stage_type.hpp>
#include <sge/shader/scoped.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/renderer/scoped_texture.hpp>
#include <sge/renderer/glsl/scoped_program.hpp>
#include <sge/parse/json/array.hpp>
#include <sge/parse/json/object.hpp>
#include <sge/parse/json/value.hpp>
#include <sge/parse/json/get.hpp>
#include <sge/camera/parameters.hpp>
#include <sge/camera/projection/perspective.hpp>
#include <sge/camera/identity_gizmo.hpp>
#include <sge/renderer/aspect.hpp>
#include <sge/shader/vf_to_string.hpp>
#include <sge/shader/variable_sequence.hpp>
#include <sge/shader/variable.hpp>
#include <sge/shader/variable_type.hpp>
#include <sge/shader/sampler_sequence.hpp>
#include <sge/shader/sampler.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/renderer/vector3.hpp>
#include <fcppt/text.hpp>
#include <fcppt/algorithm/map.hpp>
#include <fcppt/math/deg_to_rad.hpp>
#include <fcppt/math/box/basic_impl.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/math/vector/arithmetic.hpp>
#include <fcppt/math/matrix/basic_impl.hpp>
#include <fcppt/math/matrix/arithmetic.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/string.hpp>
#include <fcppt/assert.hpp>
#include <fcppt/algorithm/ptr_container_erase.hpp>
#include <fcppt/assert_message.hpp>
#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <boost/foreach.hpp>
#include <boost/statechart/event_base.hpp>
#include <iostream>



namespace
{
fruitcut::app::fruit_prototype const
parse_fruit(
	sge::parse::json::value const &v,
	sge::model::loader &model_loader,
	sge::image2d::multi_loader &image_loader,
	sge::renderer::device &renderer)
{
	sge::parse::json::object const &o = 
		sge::parse::json::get<sge::parse::json::object>(
			v);

	return 
		fruitcut::app::fruit_prototype(
			fruitcut::app::model_to_mesh(
				model_loader.load(
					fruitcut::media_path()
						/ FCPPT_TEXT("models")
						/ FCPPT_TEXT("fruits")
						/
							fruitcut::json::find_member<fcppt::string>(
								o,
								FCPPT_TEXT("model")))),
			renderer.create_texture(
				image_loader.load(
					fruitcut::media_path()
						/ FCPPT_TEXT("textures")
						/ FCPPT_TEXT("fruits")
						/
							fruitcut::json::find_member<fcppt::string>(
								o,
								FCPPT_TEXT("texture")))->view(),
				sge::renderer::filter::linear,
				sge::renderer::resource_flags::none));
}
}

fruitcut::app::states::ingame::ingame(
	my_context ctx)
:
	my_base(
		ctx),
	toggle_pause_connection_(
		context<machine>().game_input_state().key_callback(
			sge::input::keyboard::action(
				sge::input::keyboard::key_code::p, 
				boost::bind(
					// Fucking missing overload resolution
					static_cast<void (ingame::*)(boost::statechart::event_base const &)>(
						&ingame::post_event),
					this,
					events::toggle_pause())))),
	toggle_camera_connection_(
		context<machine>().game_input_state().key_callback(
			sge::input::keyboard::action(
				sge::input::keyboard::key_code::f2, 
				boost::bind(
					&ingame::toggle_camera,
					this)))),
	camera_state_(
		context<machine>().input_manager()),
	camera_(
		sge::camera::parameters(
			sge::camera::projection::perspective(
				sge::renderer::aspect<sge::renderer::scalar>(
					context<machine>().systems().renderer()->screen_size()),
				fcppt::math::deg_to_rad(
					json::find_member<sge::renderer::scalar>(
						context<machine>().config_file(),
						FCPPT_TEXT("ingame/camera/fov"))),
				json::find_member<sge::renderer::scalar>(
					context<machine>().config_file(),
					FCPPT_TEXT("ingame/camera/near")),
				json::find_member<sge::renderer::scalar>(
					context<machine>().config_file(),
					FCPPT_TEXT("ingame/camera/far"))),
			json::find_member<sge::renderer::scalar>(
				context<machine>().config_file(),
				FCPPT_TEXT("ingame/camera/movement-speed")),
			// mousespeed
			json::find_member<sge::renderer::scalar>(
				context<machine>().config_file(),
				FCPPT_TEXT("ingame/camera/mouse-speed")),
			// position
			sge::camera::identity_gizmo(),
			// Maus und Keyboard
			context<machine>().game_input_state(),
			context<machine>().game_input_state(),
			sge::camera::activation_state::inactive)),
	prototypes_(
		fcppt::algorithm::map<prototype_sequence>(
			json::find_member<sge::parse::json::array>(
				context<machine>().config_file(),
				FCPPT_TEXT("fruits")).elements,
			boost::bind(
				&parse_fruit,
				_1,
				boost::ref(
					*context<machine>().systems().md3_loader()),
				boost::ref(
					context<machine>().systems().image_loader()),
				boost::ref(
					*context<machine>().systems().renderer())))),
	physics_world_(
		// The box is ignored for now
		physics::box(),
		json::find_member<physics::vector3>(
			context<machine>().config_file(),
			FCPPT_TEXT("physics/gravity"))),
	physics_debugger_(
		physics_world_,
		context<machine>().systems().renderer(),
		camera_),
	physics_debugger_connection_(
		context<machine>().game_input_state().key_callback(
			sge::input::keyboard::action(
				sge::input::keyboard::key_code::f3, 
				boost::bind(
					&ingame::toggle_physics_debugger,
					this)))),
	collision_filter_(
		physics_world_),
	fruits_(),
	fruit_shader_(
		context<machine>().systems().renderer(),
		media_path()/FCPPT_TEXT("shaders")/FCPPT_TEXT("model_vertex.glsl"),
		media_path()/FCPPT_TEXT("shaders")/FCPPT_TEXT("model_fragment.glsl"),
		sge::shader::vf_to_string<model_vf::format>(),
		fcppt::assign::make_container<sge::shader::variable_sequence>(
			sge::shader::variable(
				"mvp",
				sge::shader::variable_type::uniform,
				sge::renderer::matrix4())),
		fcppt::assign::make_container<sge::shader::sampler_sequence>(
			sge::shader::sampler(
				"texture",
				sge::renderer::texture_ptr()))),
	old_fruits_(),
	new_fruits_()
{
	FCPPT_ASSERT_MESSAGE(
		!prototypes_.empty(),
		FCPPT_TEXT("No fruits specified!"));
	fruits_.push_back(
		new fruit(
			prototypes_.front(),
			physics_world_,
			context<machine>().systems().renderer(),
			fruit_shader_,
			static_cast<physics::scalar>(
				100),
			physics::vector3::null(),
			physics::matrix4::identity(),
			physics::vector3::null()));
}

fruitcut::physics::world &
fruitcut::app::states::ingame::physics_world()
{
	return physics_world_;
}

fruitcut::physics::world const &
fruitcut::app::states::ingame::physics_world() const
{
	return physics_world_;
}

void
fruitcut::app::states::ingame::render_fruits()
{
	sge::renderer::state::scoped scoped_state(
		context<machine>().systems().renderer(),
		sge::renderer::state::list
			(sge::renderer::state::depth_func::less));

	sge::renderer::glsl::scoped_program scoped_shader(
		context<machine>().systems().renderer(),
		fruit_shader_.program());
	for(fruit_sequence::iterator i = fruits_.begin(); i != fruits_.end(); ++i)
	{
		sge::renderer::scoped_vertex_buffer scoped_vb(
			context<machine>().systems().renderer(),
			i->vb());

		sge::renderer::scoped_texture scoped_tex(
			context<machine>().systems().renderer(),
			i->texture(),
			static_cast<sge::renderer::stage_type>(
				0));

		fruit_shader_.set_uniform(
			"mvp",
			camera_.mvp() * i->world_transform());

		context<machine>().systems().renderer()->render(
			sge::renderer::first_vertex(
				static_cast<sge::renderer::size_type>(
					0)),
			sge::renderer::vertex_count(
				i->vb()->size()),
			sge::renderer::nonindexed_primitive_type::triangle);
	}
}

fruitcut::app::states::ingame::fruit_sequence &
fruitcut::app::states::ingame::fruits()
{
	return fruits_;
}

fruitcut::app::states::ingame::fruit_sequence const &
fruitcut::app::states::ingame::fruits() const
{
	return fruits_;
}

/**
	This is a little complicated. Why this function? Well, "write
	access" to the fruits should only occur in the ingame state. So
	there's "cut_fruit()" to cut a fruit at a specified plane. We might,
	however, iterate through all the fruits and call "cut_fruit" while
	doing that, which results in undefined behaviour.

	So there are two auxiliary containers: new_fruits and
	old_fruits. You have to call the function below so new fruits are
	inserted and old ones are deleted!
 */
void
fruitcut::app::states::ingame::update_caches()
{
	BOOST_FOREACH(
		fruit const *old_fruit,
		old_fruits_)
		fcppt::algorithm::ptr_container_erase(
			fruits_,
			// ptr_container_erase doesn't like it when it gets a "T const
			// *", but we know it's safe because ingame is the only class
			// having write-access to the fruits
			const_cast<fruit *>(
				old_fruit));
	old_fruits_.clear();

	fruits_.transfer(
		fruits_.end(),
		new_fruits_);
	FCPPT_ASSERT(
		new_fruits_.empty());
}

fruitcut::physics::debugger &
fruitcut::app::states::ingame::physics_debugger()
{
	return physics_debugger_;
}

sge::camera::object &
fruitcut::app::states::ingame::camera()
{
	return camera_;
}


sge::camera::object const &
fruitcut::app::states::ingame::camera() const
{
	return camera_;
}

void
fruitcut::app::states::ingame::cut_fruit(
	fruit const &current_fruit,
	plane const &original_plane)
{
	fcppt::container::array<plane,2> planes =
		{{
			original_plane,
			plane(
				-original_plane.normal(),
				-original_plane.lambda())	
		}};

	// make_array here to be even cooler? :>
	BOOST_FOREACH(
		plane const &p,
		planes)
	{
		mesh split_mesh;
		box3 bounding_box;
		sge::renderer::vector3 barycenter;
		cut_mesh(
			current_fruit.mesh(),
			p,
			split_mesh,
			bounding_box,
			barycenter);

		if (split_mesh.triangles.empty())
			continue;
		// push here, then move new_fruits to fruits (else we're inserting while we're iterating -> not good)
		new_fruits_.push_back(
			new fruit(
				current_fruit.texture(),
				physics_world_,
				context<machine>().systems().renderer(),
				fruit_shader_,
				split_mesh,
				static_cast<physics::scalar>(
					current_fruit.bounding_box().dimension().content() / bounding_box.dimension().content()),
				current_fruit.position() + barycenter,
				current_fruit.body().rotation(),
				fcppt::math::vector::structure_cast<physics::vector3>(p.normal())
				/*current_fruit.body().linear_velocity()*/));
	}

	old_fruits_.push_back(
		&current_fruit);
}

fruitcut::app::states::ingame::~ingame()
{
}

// FIXME: This could be a nice phoenix actor
void
fruitcut::app::states::ingame::toggle_camera()
{
	camera_.activation(
		camera_.activation() == sge::camera::activation_state::active
		?
			sge::camera::activation_state::inactive
		:
			sge::camera::activation_state::active);
}

// FIXME: This could be a nice phoenix actor
void
fruitcut::app::states::ingame::toggle_physics_debugger()
{
	physics_debugger_.active(
		!physics_debugger_.active());
	std::cout << "Debug drawer is now: " << physics_debugger_.active() << "\n";
}
