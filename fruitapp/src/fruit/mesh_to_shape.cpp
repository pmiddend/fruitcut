#include <fruitapp/fruit/mesh.hpp>
#include <fruitapp/fruit/mesh_to_shape.hpp>
#include <fruitlib/physics/shared_shape_ptr.hpp>
#include <fcppt/make_shared_ptr.hpp>
#include <fcppt/config/external_begin.hpp>
#include <BulletCollision/CollisionShapes/btConvexHullShape.h>
#include <vector>
#include <fcppt/config/external_end.hpp>


fruitlib::physics::shared_shape_ptr const
fruitapp::fruit::mesh_to_shape(
	mesh const &m)
{
	typedef
	std::vector<btScalar>
	scalar_vector;

	scalar_vector points;
	points.reserve(
		static_cast<scalar_vector::size_type>(
			m.triangles().size() * 3 * 3));

	// Hehehehe
	for(
		fruit::mesh::triangle_sequence::const_iterator t =
			m.triangles().begin();
		t != m.triangles().end();
		++t)
		for(
			fruit::triangle::vertex_array::const_iterator vec =
				t->vertices.begin();
			vec != t->vertices.end();
			++vec)
			for(
				auto const &element
				:
				vec->storage()
			)
				points.push_back(
					element
				);

	return
		fcppt::make_shared_ptr<btConvexHullShape>(
			points.data(),
			// This is the number of _points_, not the number of scalars!
			static_cast<int>(
				points.size()/3),
			// Stride. Is usually sizeof(btVector3), but I think this is safer
			static_cast<int>(
				3 * sizeof(btScalar)));
}
