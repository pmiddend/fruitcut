#include <fruitapp/exception.hpp>
#include <fruitapp/fruit/box3.hpp>
#include <fruitapp/fruit/cut_mesh.hpp>
#include <fruitapp/fruit/manager.hpp>
#include <fruitapp/fruit/mesh.hpp>
#include <fruitapp/fruit/object_parameters.hpp>
#include <fruitapp/fruit/parameters_from_prototype.hpp>
#include <fruitapp/fruit/plane.hpp>
#include <fruitapp/fruit/prototype_from_json.hpp>
#include <fruitapp/fruit/model_vf/format.hpp>
#include <fruitlib/media_path.hpp>
#include <fruitlib/math/box_radius.hpp>
#include <fruitlib/math/multiply_matrix4_vector3.hpp>
#include <fruitlib/math/plane/basic.hpp>
#include <fruitlib/math/plane/distance_to_point.hpp>
#include <fruitlib/math/plane/normalize.hpp>
#include <fruitlib/physics/vector3.hpp>
#include <fruitlib/physics/rigid_body/mass.hpp>
#include <sge/camera/base.hpp>
#include <sge/camera/coordinate_system/object.hpp>
#include <sge/camera/matrix_conversion/world_projection.hpp>
#include <sge/parse/json/array.hpp>
#include <sge/parse/json/object.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/matrix4.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/renderer/vector3.hpp>
#include <sge/renderer/vector4.hpp>
#include <sge/renderer/vertex_declaration.hpp>
#include <sge/renderer/vf/dynamic/make_format.hpp>
#include <fcppt/cref.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/string.hpp>
#include <fcppt/text.hpp>
#include <fcppt/unique_ptr.hpp>
#include <fcppt/assert/error.hpp>
#include <fcppt/assign/make_array.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/container/array.hpp>
#include <fcppt/container/ptr/push_back_unique_ptr.hpp>
#include <fcppt/math/matrix/object_impl.hpp>
#include <fcppt/math/vector/length.hpp>
#include <fcppt/math/vector/normalize.hpp>
#include <fcppt/math/vector/object_impl.hpp>
#include <fcppt/math/vector/structure_cast.hpp>
#include <fcppt/signal/auto_connection.hpp>
#include <fcppt/tr1/functional.hpp>
#include <fcppt/config/external_begin.hpp>
#include <iostream>
#include <fcppt/config/external_end.hpp>


namespace
{
fruitlib::physics::vector3 const
calculate_new_linear_velocity(
	fruitlib::physics::vector3 const &old_velocity,
	fruitlib::physics::vector3 const &normal_velocity)
{
	fruitlib::physics::scalar const old_to_new_velocity_factor =
		static_cast<fruitlib::physics::scalar>(
			0.5);

	return
		old_to_new_velocity_factor * old_velocity +
			(1 - old_to_new_velocity_factor) *
				fcppt::math::vector::length(
					old_velocity) *
					normal_velocity;

}
}

fruitapp::fruit::manager::manager(
	fruitlib::scenic::optional_parent const &_parent,
	fruit::prototype_sequence const &_prototypes,
	sge::renderer::device &_renderer,
	fruitlib::physics::world &physics_world,
	sge::camera::base const &_camera,
	fruitapp::ingame_clock const &_clock)
:
	node_base(
		_parent),
	prototypes_(
		_prototypes),
	renderer_(
		_renderer),
	camera_(
		_camera),
	vertex_declaration_(
		renderer_.create_vertex_declaration(
			sge::renderer::vf::dynamic::make_format<model_vf::format>())),
	physics_world_(
		physics_world),
	fruit_group_(
		physics_world_),
	fruits_(),
	cut_signal_(),
	remove_signal_(),
	clock_(
		_clock)
{
	if(prototypes_.empty())
		throw fruitapp::exception(FCPPT_TEXT("You didn't specify any fruits"));
}


void
fruitapp::fruit::manager::cut(
	fruit::object const &current_fruit,
	plane const &original_plane,
	fruitlib::physics::vector3 const &cut_direction,
	fruitapp::ingame_clock::duration const &lock_duration)
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
	fruit::area::value_type cumulated_area = 0;
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
			cut_result->area().get();

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
				fruit::object_parameters(
					current_fruit.prototype(),
					physics_world_,
					renderer_,
					*vertex_declaration_,
					cut_result->mesh(),
					fruit_group_,
					static_cast<fruitlib::physics::rigid_body::mass::value_type>(
						cut_result->bounding_box().size().content()),
					current_fruit.position() +
						fruitlib::math::multiply_matrix4_vector3(
							current_fruit.body().transformation(),
							fcppt::math::vector::structure_cast<fruitlib::physics::vector3>(
								cut_result->barycenter())),
					current_fruit.body().transformation(),
					calculate_new_linear_velocity(
						current_fruit.body().linear_velocity(),
						fruitlib::math::multiply_matrix4_vector3(
							current_fruit.body().transformation(),
							fcppt::math::vector::structure_cast<fruitlib::physics::vector3>(
								fcppt::math::vector::normalize(
									p->normal())))),
					current_fruit.body().angular_velocity(),
					lock_duration,
					clock_)));
	}

	FCPPT_ASSERT_ERROR(
		fruit_cache.size() == 2);

	cut_signal_(
		fruit::cut_context(
			current_fruit,
			fcppt::assign::make_array<fruit::object const *>
				(&(*fruit_cache.begin()))
				(&(*(--fruit_cache.end()))),
			fruit::area(
				cumulated_area),
			cut_direction,
			cross_section));

	fruits_.transfer_from(
		fruit_cache);
	fruits_.erase(
		current_fruit);
}

void
fruitapp::fruit::manager::spawn(
	prototype const &proto,
	fruitlib::physics::scalar const mass,
	fruitlib::physics::vector3 const &position,
	fruitlib::physics::vector3 const &linear_velocity,
	fruitlib::physics::vector3 const &angular_velocity)
{
	fruits_.push_back(
		fcppt::make_unique_ptr<object>(
			fruit::parameters_from_prototype(
				proto,
				physics_world_,
				fruit_group_,
				renderer_,
				*vertex_declaration_,
				mass,
				position,
				// I don't see any sense in specifying that here
				fruitlib::physics::matrix4::identity(),
				linear_velocity,
				angular_velocity,
				fcppt::cref(
					clock_))));

	spawn_signal_(
		*fruits_.cend());
}

fruitapp::fruit::object_sequence const &
fruitapp::fruit::manager::fruits() const
{
	return fruits_;
}

fruitapp::fruit::prototype_sequence const &
fruitapp::fruit::manager::prototypes() const
{
	return prototypes_;
}

fcppt::signal::auto_connection
fruitapp::fruit::manager::cut_callback(
	fruitapp::fruit::callbacks::cut const &cc)
{
	return
		cut_signal_.connect(
			cc);
}

fcppt::signal::auto_connection
fruitapp::fruit::manager::remove_callback(
	fruitapp::fruit::callbacks::remove const &cc)
{
	return
		remove_signal_.connect(
			cc);
}

fcppt::signal::auto_connection
fruitapp::fruit::manager::spawn_callback(
	fruitapp::fruit::callbacks::spawn const &cc)
{
	return
		spawn_signal_.connect(
			cc);
}

fruitlib::physics::group::object &
fruitapp::fruit::manager::fruit_group()
{
	return fruit_group_;
}

fruitlib::physics::group::object const &
fruitapp::fruit::manager::fruit_group() const
{
	return fruit_group_;
}

sge::renderer::vertex_declaration &
fruitapp::fruit::manager::vertex_declaration()
{
	return *vertex_declaration_;
}

sge::renderer::vertex_declaration const &
fruitapp::fruit::manager::vertex_declaration() const
{
	return *vertex_declaration_;
}

fruitapp::fruit::manager::~manager()
{
}

void
fruitapp::fruit::manager::react(
	fruitlib::scenic::events::update const &)
{
	fruits_.update();
	delete_distant_fruits();
}

void
fruitapp::fruit::manager::delete_distant_fruits()
{
	typedef
	fruitlib::math::plane::basic<sge::renderer::scalar,3>
	plane_type;

	sge::renderer::matrix4 const mvp =
		sge::camera::matrix_conversion::world_projection(
			camera_.coordinate_system(),
			camera_.projection_matrix());

	sge::renderer::vector4 const
		fourth_row(
			mvp[3][0],
			mvp[3][1],
			mvp[3][2],
			mvp[3][3]),
		second_row(
			mvp[1][0],
			mvp[1][1],
			mvp[1][2],
			mvp[1][3]),
		plane_vec4 =
			fourth_row - second_row;

	plane_type const bottom_plane =
		fruitlib::math::plane::normalize(
			plane_type(
				sge::renderer::vector3(
					plane_vec4[0],
					plane_vec4[1],
					plane_vec4[2]),
				plane_vec4[3]));

	for(object_sequence::iterator i = fruits_.begin(); i != fruits_.end(); ++i)
	{
		if(
			fruitlib::math::plane::distance_to_point(bottom_plane,i->position()) >
			// This 2 is important here. If it weren't there, we would delete fruits which were just spawned.
			static_cast<sge::renderer::scalar>(2) *
			fruitlib::math::box_radius(
				i->bounding_box()))
		{
			remove_signal_(
				*i);
			fruits_.erase(
				*i);
		}
	}
	fruits_.update();
}
