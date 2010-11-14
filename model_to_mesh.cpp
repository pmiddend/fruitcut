#include "model_to_mesh.hpp"
#include "triangle.hpp"
#include "vec2.hpp"
#include "vec3.hpp"
#include <sge/model/object.hpp>
#include <sge/model/vertex_sequence.hpp>
#include <sge/model/texcoord_sequence.hpp>
#include <boost/foreach.hpp>
#include <fcppt/assert.hpp>
#include <fcppt/string.hpp>

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
		result.triangles.push_back(
			triangle(
				{
					fcppt::math::vector::structure_cast<vec3>(
						vertices[*index]),
					fcppt::math::vector::structure_cast<vec3>(
						vertices[*(index+1)]),
					fcppt::math::vector::structure_cast<vec3>(
						vertices[*(index+2)])
				},
				{
					fcppt::math::vector::structure_cast<vec2>(
						texcoords[*index]),
					fcppt::math::vector::structure_cast<vec2>(
						texcoords[*(index+1)]),
					fcppt::math::vector::structure_cast<vec2>(
						texcoords[*(index+2)])
				}));
	}

	return result;
}
