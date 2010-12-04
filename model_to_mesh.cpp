#include "model_to_mesh.hpp"
#include "triangle.hpp"
#include <sge/renderer/vector2.hpp>
#include <sge/renderer/vector3.hpp>
#include <sge/model/object.hpp>
#include <sge/model/vertex_sequence.hpp>
#include <sge/model/texcoord_sequence.hpp>
#include <fcppt/math/vector/structure_cast.hpp>
#include <fcppt/assert.hpp>
#include <fcppt/string.hpp>
#include <boost/foreach.hpp>

fruitcut::mesh const
fruitcut::model_to_mesh(
	sge::model::object_ptr const model)
{
	mesh result;
	FCPPT_ASSERT(
		!model->part_names().empty());
	fcppt::string const part_name = model->part_names().front();
	sge::model::vertex_sequence const vertices = 
		model->vertices(
			part_name);
	FCPPT_ASSERT(
		model->texcoords(
			part_name));
	sge::model::texcoord_sequence const texcoords = 
		*model->texcoords(
			part_name);

	sge::model::index_sequence const indices = 
		model->indices(
			part_name);

	FCPPT_ASSERT(
		vertices.size() == texcoords.size());

	FCPPT_ASSERT(
		indices.size() % 3 == 0);

	for(
		sge::model::index_sequence::const_iterator index = indices.begin();
		index != indices.end();
		index += 3)
	{
		triangle::vertex_array vt;
		triangle::texcoord_array tc;
		for (triangle::vertex_array::size_type i = 0; i < vt.size(); ++i)
		{
			vt[i] = 
				fcppt::math::vector::structure_cast<sge::renderer::vector3>(
					vertices[
						*boost::next(
							index,
							i)]);

			sge::renderer::vector2 const current_coord = 
				fcppt::math::vector::structure_cast<sge::renderer::vector2>(
					texcoords[
						*boost::next(
							index,
							i)]);
				
			tc[i] = 
				sge::renderer::vector2(
					current_coord.x()/* / 2*/,
					current_coord.y());
		}
		result.triangles.push_back(
			triangle(
				vt,
				tc));
	}

	return result;
}
