#include <fruitapp/exception.hpp>
#include <fruitapp/media_path.hpp>
#include <fcppt/math/matrix/inverse.hpp>
#include <fruitapp/fruit/box3.hpp>
#include <fruitapp/fruit/cut_context_unique_ptr.hpp>
#include <fruitapp/fruit/cut_mesh.hpp>
#include <fruitapp/fruit/manager.hpp>
#include <fruitapp/fruit/mesh.hpp>
#include <fruitapp/fruit/object_from_prototype.hpp>
#include <fruitapp/fruit/plane.hpp>
#include <fruitapp/fruit/prototype_from_json.hpp>
#include <fruitapp/fruit/model_vf/format.hpp>
#include <fruitlib/math/box_radius.hpp>
#include <fruitlib/math/unproject.hpp>
#include <fruitlib/math/plane/basic.hpp>
#include <fruitlib/math/plane/distance_to_point.hpp>
#include <fruitlib/math/plane/normalize.hpp>
#include <fruitlib/physics/vector3.hpp>
#include <fruitlib/physics/rigid_body/mass.hpp>
#include <sge/camera/base.hpp>
#include <sge/camera/coordinate_system/object.hpp>
#include <sge/camera/matrix_conversion/world_projection.hpp>
#include <sge/parse/json/array.hpp>
#include <sge/renderer/target/base.hpp>
#include <sge/parse/json/object.hpp>
#include <sge/renderer/matrix4.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/renderer/vector3.hpp>
#include <sge/renderer/vector4.hpp>
#include <sge/renderer/vertex_declaration.hpp>
#include <sge/renderer/device/core.hpp>
#include <sge/renderer/vf/dynamic/make_format.hpp>
#include <fcppt/cref.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/ref.hpp>
#include <fcppt/string.hpp>
#include <fcppt/text.hpp>
#include <fcppt/unique_ptr.hpp>
#include <fcppt/assert/error.hpp>
#include <fcppt/assign/make_array.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/container/array.hpp>
#include <fcppt/container/ptr/push_back_unique_ptr.hpp>
#include <fcppt/math/matrix/object_impl.hpp>
#include <fcppt/math/matrix/multiply_matrix4_vector3.hpp>
#include <fcppt/math/matrix/transpose.hpp>
#include <fcppt/math/vector/length.hpp>
#include <fcppt/math/vector/normalize.hpp>
#include <fcppt/math/vector/cross.hpp>
#include <fcppt/math/vector/dot.hpp>
#include <fcppt/math/vector/object_impl.hpp>
#include <fcppt/math/vector/structure_cast.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <fruitapp/renderer_rect.hpp>
#include <fruitapp/renderer_dim2.hpp>
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
	fruitapp::fruit::prototype_sequence const &_prototypes,
	sge::renderer::device::core &_renderer,
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
			sge::renderer::vf::dynamic::make_format<fruitapp::fruit::model_vf::format>())),
	physics_world_(
		physics_world),
	fruit_group_(
		physics_world_),
	fruits_(),
	cut_signal_(),
	remove_signal_(),
	spawn_signal_(),
	clock_(
		_clock)
{
	if(prototypes_.empty())
		throw
			fruitapp::exception(
				FCPPT_TEXT("You didn't specify any fruits"));
}


void
fruitapp::fruit::manager::cut(
	fruitapp::fruit::object const &_current_fruit,
	fruitapp::fruit::hull::intersection_pair const &_intersection,
	fruitapp::fruit::ban_duration const &_ban_duration,
	sge::renderer::target::base const &_target)
{
	if(!_intersection || _current_fruit.locked())
		return;

	sge::renderer::matrix4 const inverse_mvp =
		fcppt::math::matrix::inverse(
			sge::camera::matrix_conversion::world_projection(
				camera_.coordinate_system(),
				camera_.projection_matrix()));

	sge::renderer::vector3 const
		// Convert the points to 3D and to renderer::scalar
		point1(
			static_cast<sge::renderer::scalar>(
				_intersection->first[0]),
			static_cast<sge::renderer::scalar>(
				_intersection->first[1]),
			static_cast<sge::renderer::scalar>(
				0)),
		point2(
			static_cast<sge::renderer::scalar>(
				_intersection->second[0]),
			static_cast<sge::renderer::scalar>(
				_intersection->second[1]),
			static_cast<sge::renderer::scalar>(
				0)),
		// unproject 'em
		point1_unprojected =
			fruitlib::math::unproject(
				point1,
				inverse_mvp,
				// The points are already "un-viewported", but they are in
				// screenspace, so use the screen rect here
				fruitapp::renderer_rect(
					sge::renderer::vector2::null(),
					fcppt::math::dim::structure_cast<fruitapp::renderer_dim2>(
						_target.viewport().get().size()))),
		point2_unprojected =
			fruitlib::math::unproject(
				point2,
				inverse_mvp,
				// The points are already "un-viewported", but they are in
				// screenspace, so use the screen rect here
				fruitapp::renderer_rect(
					sge::renderer::vector2::null(),
					fcppt::math::dim::structure_cast<fruitapp::renderer_dim2>(
						_target.viewport().get().size()))),
		point3_unprojected =
			fruitlib::math::unproject(
				sge::renderer::vector3(
					point1.x(),
					point1.y(),
					static_cast<sge::renderer::scalar>(
						0.5)),
				inverse_mvp,
				fruitapp::renderer_rect(
					sge::renderer::vector2::null(),
					fcppt::math::dim::structure_cast<fruitapp::renderer_dim2>(
						_target.viewport().get().size()))),
		first_plane_vector =
			point2_unprojected - point1_unprojected,
		second_plane_vector =
			point3_unprojected - point1_unprojected,
		// NOTE: For rotation matrices M and vectors a,b the following holds:
		// cross(M*a,M*b) = M*cross(a,b)
		plane_normal =
		fcppt::math::matrix::multiply_matrix4_vector3(
				fcppt::math::matrix::transpose(
					_current_fruit.rotation()),
				fcppt::math::vector::cross(
					first_plane_vector,
					second_plane_vector));

	sge::renderer::scalar const plane_scalar =
		fcppt::math::vector::dot(
			fcppt::math::matrix::multiply_matrix4_vector3(
				fcppt::math::matrix::transpose(
					_current_fruit.rotation()),
				point1_unprojected - _current_fruit.position()),
			plane_normal);

	fruitapp::fruit::plane const original_plane(
		plane_normal,
		plane_scalar);

	sge::renderer::vector3 const cut_direction(
		first_plane_vector);

	typedef
	fcppt::container::array<fruitapp::fruit::plane,2>
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
	fruitapp::fruit::object_sequence::implementation_sequence fruit_cache;
	fruitapp::fruit::area::value_type cumulated_area = 0;
	fruitapp::fruit::mesh_unique_ptr cross_section(
		fcppt::make_unique_ptr<fruit::mesh>(
			fruitapp::fruit::mesh::triangle_sequence()));

	for(
		plane_array::const_iterator p =
			planes.begin();
		p != planes.end();
		++p)
	{
		fcppt::unique_ptr<fruitapp::fruit::cut_mesh_result> cut_result(
			fruitapp::fruit::cut_mesh(
				_current_fruit.mesh(),
				*p));

		cumulated_area +=
			cut_result->area().get();

		// Note the return here. If this condition is true, we only split
		// to one fruit, so we didn't split at all!
		if (cut_result->mesh().triangles().empty())
			return;

		// Potentially dangerous if cross_section is used by something
		// else. But currently this is the fastest solution.
		if (cross_section->triangles().empty())
			cut_result->cross_section().triangles().swap(
				cross_section->triangles());

		fcppt::container::ptr::push_back_unique_ptr(
			fruit_cache,
			fcppt::make_unique_ptr<fruitapp::fruit::object>(
				fcppt::cref(
					_current_fruit.prototype()),
				fcppt::ref(
					physics_world_),
				fcppt::ref(
					renderer_),
				fcppt::ref(
					*vertex_declaration_),
				cut_result->release_mesh(),
				fcppt::ref(
					fruit_group_),
				static_cast<fruitlib::physics::rigid_body::mass::value_type>(
					cut_result->bounding_box().size().content()),
				_current_fruit.position() +
					fcppt::math::matrix::multiply_matrix4_vector3(
						_current_fruit.body().transformation(),
						fcppt::math::vector::structure_cast<fruitlib::physics::vector3>(
							cut_result->barycenter())),
				_current_fruit.body().transformation(),
				calculate_new_linear_velocity(
					_current_fruit.body().linear_velocity(),
					fcppt::math::matrix::multiply_matrix4_vector3(
						_current_fruit.body().transformation(),
						fcppt::math::vector::structure_cast<fruitlib::physics::vector3>(
							fcppt::math::vector::normalize(
								p->normal())))),
				_current_fruit.body().angular_velocity(),
				_ban_duration,
				fcppt::cref(
					clock_)));
	}

	FCPPT_ASSERT_ERROR(
		fruit_cache.size() == 2);

	fruit::cut_context_unique_ptr cut_context(
		fcppt::make_unique_ptr<fruit::cut_context>(
			fcppt::cref(
				_current_fruit),
			fcppt::assign::make_array<fruit::object const *>
				(&(*fruit_cache.begin()))
				(&(*(--fruit_cache.end()))),
			fruit::area(
				cumulated_area),
			cut_direction,
			fcppt::move(
				cross_section)));

	cut_signal_(
		fcppt::cref(
			*cut_context));

	fruits_.transfer_from(
		fruit_cache);

	fruits_.erase(
		_current_fruit);
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
		fruit::object_from_prototype(
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
				clock_)));

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
