#include "fruit.hpp"
#include "fruit_prototype.hpp"
#include "mesh_to_vertex_buffer.hpp"
#include "mesh_to_shape.hpp"
#include "../geometry_traits/box.hpp"
#include "../geometry_traits/vector.hpp"
#include "../physics/rigid_body/parameters.hpp"
#include "../physics/world_fwd.hpp"
#include <sge/renderer/device_ptr.hpp>
#include <sge/shader/object_fwd.hpp>
#include <fcppt/math/matrix/structure_cast.hpp>
#include <boost/geometry/geometry.hpp>
#include <boost/geometry/multi/multi.hpp>
#include <boost/foreach.hpp>
#include <vector>

namespace
{
// The make_envelope function only accepts ranges, to we convert the
// mesh to one here.
// TODO: We could also write a range adapter for mesh: Keep an index
// to the current point and an index to the current triangle, should
// be pretty simple
typedef
boost::geometry::model::multi_point<fruitcut::app::triangle::vector>
point_cloud;

point_cloud const
mesh_to_point_sequence(
	fruitcut::app::mesh const &m)
{
	point_cloud output;
	output.reserve(
		static_cast<point_cloud::size_type>(
			m.triangles.size() * 3));

	BOOST_FOREACH(
		fruitcut::app::triangle const &t,
		m.triangles)
		BOOST_FOREACH(
			fruitcut::app::triangle::vertex_array::const_reference r,
			t.vertices)
			output.push_back(
				r);

	return output;
}
}

fruitcut::app::fruit::fruit(
	fruit_prototype const &proto,
	physics::world &_world,
	sge::renderer::device_ptr const _renderer,
	sge::shader::object &_shader,
	physics::scalar const _mass,
	physics::vector3 const &_position,
	physics::matrix4 const &_transformation,
	physics::vector3 const &_linear_velocity)
:
	mesh_(
		proto.mesh()),
	bounding_box_(
		boost::geometry::make_envelope<box3>(
			mesh_to_point_sequence(
				mesh_))),
	body_(
		physics::rigid_body::parameters(
			_world,
			_position,
			_transformation,
			_linear_velocity,
			mesh_to_shape(
				mesh_),
			physics::rigid_body::solidity::solid,
			_mass)),
	vb_(
		mesh_to_vertex_buffer(
			_renderer,
			_shader,
			mesh_)),
	texture_(
		proto.texture())
{
}

sge::renderer::vertex_buffer_ptr const
fruitcut::app::fruit::vb() const
{
	return vb_;
}

sge::renderer::texture_ptr const
fruitcut::app::fruit::texture() const
{
	return texture_;
}

sge::renderer::matrix4 const
fruitcut::app::fruit::world_transform() const
{
	return 
		fcppt::math::matrix::structure_cast<sge::renderer::matrix4>(
			body_.world_transform());
}

fruitcut::app::box3 const &
fruitcut::app::fruit::bounding_box() const
{
	return bounding_box_;
}

fruitcut::app::fruit::~fruit()
{
}
