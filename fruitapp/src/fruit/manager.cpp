#include <fruitapp/exception.hpp>
#include <fruitapp/media_path.hpp>
#include <fruitapp/fruit/box3.hpp>
#include <fruitapp/fruit/calculate_cut_geometry.hpp>
#include <fruitapp/fruit/calculate_new_linear_velocity.hpp>
#include <fruitapp/fruit/cut_context_unique_ptr.hpp>
#include <fruitapp/fruit/cut_geometry.hpp>
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
#include <sge/parse/json/object.hpp>
#include <sge/renderer/matrix4.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/renderer/vector3.hpp>
#include <sge/renderer/vector4.hpp>
#include <sge/renderer/device/core.hpp>
#include <sge/renderer/target/base.hpp>
#include <sge/renderer/vertex/declaration.hpp>
#include <sge/renderer/vertex/declaration_parameters.hpp>
#include <sge/renderer/vf/dynamic/make_format.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/string.hpp>
#include <fcppt/text.hpp>
#include <fcppt/unique_ptr.hpp>
#include <fcppt/assert/error.hpp>
#include <fcppt/cast/size_fun.hpp>
#include <fcppt/math/dim/contents.hpp>
#include <fcppt/math/matrix/identity.hpp>
#include <fcppt/math/matrix/object_impl.hpp>
#include <fcppt/math/matrix/transform_direction.hpp>
#include <fcppt/math/matrix/transpose.hpp>
#include <fcppt/math/vector/normalize.hpp>
#include <fcppt/math/vector/object_impl.hpp>
#include <fcppt/math/vector/structure_cast.hpp>
#include <fcppt/signal/auto_connection.hpp>
#include <fcppt/config/external_begin.hpp>
#include <array>
#include <iterator>
#include <utility>
#include <fcppt/config/external_end.hpp>


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
			sge::renderer::vertex::declaration_parameters(
				sge::renderer::vf::dynamic::make_format<fruitapp::fruit::model_vf::format>()))),
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
	fruitapp::fruit::hull::optional_intersection_pair const &_intersection,
	fruitapp::fruit::ban_duration const &_ban_duration,
	sge::renderer::target::base const &_target)
{
	if(!_intersection.has_value() || _current_fruit.locked())
		return;

	fruitapp::fruit::cut_geometry const cut_geometry(
		fruitapp::fruit::calculate_cut_geometry(
			_intersection.get_unsafe(), // TODO
			_target,
			camera_,
			_current_fruit));

	typedef
	std::array<fruitapp::fruit::plane,2u>
	plane_array;

	plane_array const planes =
		{{
			cut_geometry.cut_plane().get(),
			fruitapp::fruit::plane(
				-cut_geometry.cut_plane().get().normal(),
				-cut_geometry.cut_plane().get().lambda())
		}};

	// We have to check if we split the fruit into one or two parts. If
	// it's just one, we leave it as is (still costs a bit of performance)
	fruitapp::fruit::object_sequence::implementation_sequence fruit_cache;
	fruitapp::fruit::area cumulated_area(
		0.0f);
	fruitapp::fruit::mesh_unique_ptr cross_section(
		fcppt::make_unique_ptr<fruitapp::fruit::mesh>(
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
			cut_result->area();

		// Note the return here. If this condition is true, we only split
		// to one fruit, so we didn't split at all!
		if (cut_result->mesh().triangles().empty())
			return;

		// Potentially dangerous if cross_section is used by something
		// else. But currently this is the fastest solution.
		if (cross_section->triangles().empty())
			cut_result->cross_section().triangles().swap(
				cross_section->triangles());

		fruit_cache.push_back(
			fcppt::make_unique_ptr<fruitapp::fruit::object>(
				_current_fruit.prototype(),
				physics_world_,
				renderer_,
				*vertex_declaration_,
				cut_result->release_mesh(),
				fruit_group_,
				fruitlib::physics::rigid_body::mass(
					static_cast<fruitlib::physics::scalar>(
						fcppt::math::dim::contents(
							cut_result->bounding_box().size()
						)
					)
				),
				fruitlib::physics::rigid_body::position(
					_current_fruit.position() +
					fcppt::math::matrix::transform_direction(
						_current_fruit.body().transformation().get(),
						fcppt::math::vector::structure_cast<fruitlib::physics::vector3, fcppt::cast::size_fun>(
							cut_result->barycenter()))),
				_current_fruit.body().transformation(),
				fruitapp::fruit::calculate_new_linear_velocity(
					_current_fruit.body().linear_velocity(),
					fruitlib::physics::rigid_body::linear_velocity(
						fcppt::math::matrix::transform_direction(
							_current_fruit.body().transformation().get(),
							fcppt::math::vector::structure_cast<fruitlib::physics::vector3, fcppt::cast::size_fun>(
								fcppt::math::vector::normalize(
									p->normal()))))),
				_current_fruit.body().angular_velocity(),
				_ban_duration,
				clock_));
	}

	FCPPT_ASSERT_ERROR(
		fruit_cache.size() == 2);

	fruit::cut_context_unique_ptr cut_context(
		fcppt::make_unique_ptr<fruitapp::fruit::cut_context>(
			_current_fruit,
			fruitapp::fruit::cut_context::new_fruit_array
			{{
				fruit_cache.begin()->get_pointer(),
				std::next(
					fruit_cache.begin()
				)->get_pointer()
			}},
			cumulated_area,
			cut_geometry,
			std::move(
				cross_section)));

	remove_signal_(
		_current_fruit);

	cut_signal_(
		*cut_context);

	fruits_.transfer_from(
		fruit_cache);

	fruits_.erase(
		_current_fruit);
}

void
fruitapp::fruit::manager::spawn(
	prototype const &proto,
	fruitlib::physics::rigid_body::mass const &mass,
	fruitlib::physics::rigid_body::position const &position,
	fruitlib::physics::rigid_body::linear_velocity const &linear_velocity,
	fruitlib::physics::rigid_body::angular_velocity const &angular_velocity)
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
			fruitlib::physics::rigid_body::transformation(
				fcppt::math::matrix::identity<
					fruitlib::physics::matrix4
				>()
			),
			linear_velocity,
			angular_velocity,
			clock_));

	spawn_signal_(
		**fruits_.cend());
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

sge::renderer::vertex::declaration const &
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

	for(
		auto const &fruit
		:
		fruits_
	)
	{
		if(
			fruitlib::math::plane::distance_to_point(bottom_plane,fruit->position()) >
			// This 2 is important here. If it weren't there, we would delete fruits which were just spawned.
			static_cast<sge::renderer::scalar>(2) *
			fruitlib::math::box_radius(
				fruit->bounding_box()))
		{
			remove_signal_(
				*fruit);
			fruits_.erase(
				*fruit);
		}
	}
	fruits_.update();
}
