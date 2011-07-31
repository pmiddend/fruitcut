#include <fruitapp/fruit/mesh_to_shape.hpp>
#include <fruitapp/fruit/mesh.hpp>
#include <fruitlib/physics/shared_shape_ptr.hpp>
#include <fcppt/container/raw_vector.hpp>
#include <fcppt/make_shared_ptr.hpp>
#include <BulletCollision/CollisionShapes/btConvexHullShape.h>
#include <iostream>

fruitlib::physics::shared_shape_ptr const
fruitapp::fruit::mesh_to_shape(
	mesh const &m)
{
	typedef
	fcppt::container::raw_vector<btScalar>
	scalar_vector;

	scalar_vector points;
	points.reserve(
		static_cast<scalar_vector::size_type>(
			m.triangles.size() * 3 * 3));

	// Hehehehe
	for(
		fruit::mesh::triangle_sequence::const_iterator t = 
			m.triangles.begin();
		t != m.triangles.end();
		++t)
		for(
			fruit::triangle::vertex_array::const_iterator vec = 
				t->vertices.begin();
			vec != t->vertices.end();
			++vec)
			for(
				fruit::triangle::vector::const_iterator r = 
					vec->begin();
				r != vec->end();
				++r)
				points.push_back(
					*r);

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
