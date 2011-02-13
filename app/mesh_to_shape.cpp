#include "mesh_to_shape.hpp"
#include "mesh.hpp"
#include "../physics/shared_shape_ptr.hpp"
#include <fcppt/container/raw_vector.hpp>
#include <bullet/BulletCollision/CollisionShapes/btConvexHullShape.h>
#include <boost/foreach.hpp>
#include <iostream>

fruitcut::physics::shared_shape_ptr const
fruitcut::app::mesh_to_shape(
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
	BOOST_FOREACH(
		triangle const &t,
		m.triangles)
		BOOST_FOREACH(
			triangle::vertex_array::const_reference vec,
			t.vertices)
			BOOST_FOREACH(
				triangle::vector::const_reference r,
				vec)
				points.push_back(
					r);

	return 
		physics::shared_shape_ptr(
			new btConvexHullShape(
				points.data(),
				// This is the number of _points_, not the number of scalars!
				static_cast<int>(
					points.size()/3),
				// Stride. Is usually sizeof(btVector3), but I think this is safer
				static_cast<int>(
					3 * sizeof(btScalar))));
}
