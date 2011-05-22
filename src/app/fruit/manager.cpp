#include "box3.hpp"
#include "cut_mesh.hpp"
#include "manager.hpp"
#include "mesh.hpp"
#include "model_to_mesh.hpp"
#include "model_vf/format.hpp"
#include "plane.hpp"
#include "../../json/find_member.hpp"
#include "../../math/multiply_matrix4_vector3.hpp"
#include "../../media_path.hpp"
#include <sge/shader/vf_to_string.hpp>
#include <sge/shader/variable_sequence.hpp>
#include <sge/shader/variable.hpp>
#include <sge/shader/variable_type.hpp>
#include <sge/shader/sampler_sequence.hpp>
#include <sge/shader/sampler.hpp>
#include <sge/shader/scoped.hpp>
#include <sge/camera/object.hpp>
#include <sge/renderer/matrix4.hpp>
#include <sge/renderer/vf/dynamic/make_format.hpp>
#include <sge/renderer/scoped_vertex_buffer.hpp>
#include <sge/renderer/scoped_vertex_declaration.hpp>
#include <sge/renderer/texture/scoped.hpp>
#include <sge/renderer/stage_type.hpp>
#include <sge/renderer/state/scoped.hpp>
#include <sge/renderer/state/var.hpp>
#include <sge/renderer/state/trampoline.hpp>
#include <sge/renderer/state/list.hpp>
#include <sge/renderer/state/depth_func.hpp>
#include <sge/renderer/texture/planar_ptr.hpp>
#include <sge/renderer/first_vertex.hpp>
#include <sge/renderer/size_type.hpp>
#include <sge/renderer/vertex_count.hpp>
#include <sge/renderer/nonindexed_primitive_type.hpp>
#include <sge/renderer/vertex_buffer.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/glsl/scoped_program.hpp>
#include <sge/renderer/texture/filter/linear.hpp>
#include <sge/renderer/texture/filter/trilinear.hpp>
#include <sge/renderer/texture/create_planar_from_view.hpp>
#include <sge/renderer/texture/address_mode.hpp>
#include <sge/renderer/texture/address_mode2.hpp>
#include <sge/renderer/texture/planar.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/renderer/vector3.hpp>
#include <sge/parse/json/get.hpp>
#include <sge/parse/json/object.hpp>
#include <sge/parse/json/array.hpp>
#include <sge/model/loader.hpp>
#include <sge/image2d/loader.hpp>
#include <sge/image2d/multi_loader.hpp>
#include <sge/image2d/file.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/algorithm/map.hpp>
#include <fcppt/tr1/functional.hpp>
#include <fcppt/math/matrix/basic_impl.hpp>
#include <fcppt/math/matrix/arithmetic.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/math/vector/structure_cast.hpp>
#include <fcppt/math/vector/length.hpp>
#include <fcppt/container/array.hpp>
#include <fcppt/assign/make_array.hpp>
#include <fcppt/container/ptr/push_back_unique_ptr.hpp>
#include <fcppt/string.hpp>
#include <fcppt/text.hpp>
#include <fcppt/ref.hpp>
#include <fcppt/assert_message.hpp>
#include <fcppt/assert.hpp>

namespace
{
fruitcut::app::fruit::prototype const
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
		fruitcut::app::fruit::prototype(
			fruitcut::app::fruit::model_to_mesh(
				model_loader.load(
					fruitcut::media_path()
						/ FCPPT_TEXT("models")
						/ FCPPT_TEXT("fruits")
						/
							fruitcut::json::find_member<fcppt::string>(
								o,
								FCPPT_TEXT("model")))),
			sge::renderer::texture::create_planar_from_view(
				renderer,
				image_loader.load(
					fruitcut::media_path()
						/ FCPPT_TEXT("textures")
						/ FCPPT_TEXT("fruits")
						/
							fruitcut::json::find_member<fcppt::string>(
								o,
								FCPPT_TEXT("texture")))->view(),
				sge::renderer::texture::filter::trilinear,
				sge::renderer::texture::address_mode2(
					sge::renderer::texture::address_mode::clamp),
				sge::renderer::resource_flags::none));
}
}

fruitcut::app::fruit::manager::manager(
	sge::parse::json::array const &prototype_array,
	sge::model::loader &model_loader,
	sge::image2d::multi_loader &image_loader,
	sge::renderer::device &_renderer,
	physics::world &_physics_world,
	sge::camera::object &_camera)
:
	renderer_(
		_renderer),
	camera_(
		_camera),
	vertex_declaration_(
		renderer_.create_vertex_declaration(
			sge::renderer::vf::dynamic::make_format<model_vf::format>())),
	physics_world_(
		_physics_world),
	prototypes_(
		fcppt::algorithm::map<prototype_sequence>(
			prototype_array.elements,
			std::tr1::bind(
				&parse_fruit,
				std::tr1::placeholders::_1,
				fcppt::ref(
					model_loader),
				fcppt::ref(
					image_loader),
				fcppt::ref(
					_renderer)))),
	fruits_(),
	fruit_shader_(
		_renderer,
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
				sge::renderer::texture::planar_ptr()))),
	cut_signal_(),
	remove_signal_()
{
	FCPPT_ASSERT_MESSAGE(
		!prototypes_.empty(),
		FCPPT_TEXT("No fruits specified!"));
}


void
fruitcut::app::fruit::manager::cut(
	fruit::object const &current_fruit,
	plane const &original_plane,
	physics::vector3 const &cut_direction,
	sge::time::duration const &lock_duration,
	sge::time::callback const &time_callback)
{
	if(current_fruit.locked())
		return;

	typedef
	fcppt::container::array<plane,2>
	plane_array;

	plane_array planes =
		{{
			original_plane,
			plane(
				-original_plane.normal(),
				-original_plane.lambda())	
		}};

	// We have to check if we split the fruit into one or two parts. If
	// it's just one, we leave it as is (still costs a bit of performance)
	object_sequence::implementation_sequence fruit_cache;
	fruit::area cumulated_area = 0;
	fruit::mesh cross_section;

	for(
		plane_array::const_iterator p = 
			planes.begin();
		p != planes.end();
		++p)
	{
		fcppt::unique_ptr<fruit::cut_mesh_result> cut_result(
			fruit::cut_mesh(
				current_fruit.mesh(),
				*p));

		cumulated_area += 
			cut_result->area();

		// Note the return here. If this condition is true, we only split
		// to one fruit, so we didn't split at all!
		if (cut_result->mesh().triangles.empty())
			return;

		// Potentially dangerous if cross_section is used by something else. But currently this is the fastest solution.
		if (cross_section.triangles.empty())
			cut_result->cross_section().triangles.swap(
				cross_section.triangles);

		fcppt::container::ptr::push_back_unique_ptr(
			fruit_cache,
			fcppt::make_unique_ptr<fruit::object>(
				current_fruit.texture(),
				fcppt::ref(
					physics_world_),
				fcppt::ref(
					renderer_),
				fcppt::ref(
					*vertex_declaration_),
				fcppt::ref(
					fruit_shader_),
				fcppt::ref(
					cut_result->mesh()),
				static_cast<physics::scalar>(
					current_fruit.bounding_box().size().content() / cut_result->bounding_box().size().content()),
				current_fruit.position() + 
					math::multiply_matrix4_vector3(
						current_fruit.body().rotation(),
						fcppt::math::vector::structure_cast<physics::vector3>(
							cut_result->barycenter())),
				current_fruit.body().rotation(),
				current_fruit.body().linear_velocity() + 
					(static_cast<physics::scalar>(0.125) * 
						fcppt::math::vector::length(
							current_fruit.body().linear_velocity()) * 
						math::multiply_matrix4_vector3(
							current_fruit.body().rotation(),
							fcppt::math::vector::structure_cast<physics::vector3>(
								p->normal()))),
				current_fruit.body().angular_velocity(),
				lock_duration,
				time_callback));
	}

	FCPPT_ASSERT(
		fruit_cache.size() == 2);

	cut_signal_(
		fruit::cut_context(
			current_fruit,
			fcppt::assign::make_array<fruit::object const *>
				(&(*fruit_cache.begin()))
				(&(*(--fruit_cache.end()))),
			cumulated_area,
			cut_direction,
			cross_section));

	fruits_.transfer_from(
		fruit_cache);
	fruits_.erase(
		current_fruit);
}

void
fruitcut::app::fruit::manager::spawn(
	prototype const &proto,
	physics::scalar const mass,
	physics::vector3 const &position,
	physics::vector3 const &linear_velocity,
	physics::vector3 const &angular_velocity)
{
	fruits_.push_back(
		fcppt::make_unique_ptr<object>(
			proto,
			fcppt::ref(
				physics_world_),
			fcppt::ref(
				renderer_),
			fcppt::ref(
				*vertex_declaration_),
			fcppt::ref(
				fruit_shader_),
			mass,
			position,
			// I don't see any sense in specifying that here
			physics::matrix4::identity(),
			linear_velocity,
			angular_velocity));

	spawn_signal_(
		*fruits_.cend());
}

fruitcut::app::fruit::object_sequence const &
fruitcut::app::fruit::manager::fruits() const
{
	return fruits_;
}

fruitcut::app::fruit::prototype_sequence const &
fruitcut::app::fruit::manager::prototypes() const
{
	return prototypes_;
}

fcppt::signal::auto_connection
fruitcut::app::fruit::manager::cut_callback(
	fruitcut::app::fruit::callbacks::cut const &cc)
{
	return 
		cut_signal_.connect(
			cc);
}

fcppt::signal::auto_connection
fruitcut::app::fruit::manager::remove_callback(
	fruitcut::app::fruit::callbacks::remove const &cc)
{
	return 
		remove_signal_.connect(
			cc);
}

fcppt::signal::auto_connection
fruitcut::app::fruit::manager::spawn_callback(
	fruitcut::app::fruit::callbacks::spawn const &cc)
{
	return 
		spawn_signal_.connect(
			cc);
}

fruitcut::app::fruit::manager::~manager()
{
}

void
fruitcut::app::fruit::manager::delete_distant_fruits()
{
	
}

void
fruitcut::app::fruit::manager::update()
{
	fruits_.update();
	delete_distant_fruits();
}

void
fruitcut::app::fruit::manager::render()
{
	sge::renderer::state::scoped scoped_state(
		renderer_,
		sge::renderer::state::list
			(sge::renderer::state::depth_func::less));

	sge::renderer::glsl::scoped_program scoped_shader(
		renderer_,
		fruit_shader_.program());

	sge::renderer::scoped_vertex_declaration scoped_decl(
		renderer_,
		*vertex_declaration_);

	for(object_sequence::iterator i = fruits_.begin(); i != fruits_.end(); ++i)
	{
		sge::renderer::scoped_vertex_buffer scoped_vb(
			renderer_,
			*i->vb());

		sge::renderer::texture::scoped scoped_tex(
			renderer_,
			*i->texture(),
			static_cast<sge::renderer::stage_type>(
				0));

		fruit_shader_.update_uniform(
			"mvp",
			camera_.mvp() * i->world_transform());

		renderer_.render(
			sge::renderer::first_vertex(
				static_cast<sge::renderer::size_type>(
					0)),
			sge::renderer::vertex_count(
				i->vb()->size()),
			sge::renderer::nonindexed_primitive_type::triangle);
	}
}
