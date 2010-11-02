#include "model_to_mesh.hpp"
#include "triangle.hpp"
#include "vec2.hpp"
#include "vec3.hpp"
#include <sge/model/object.hpp>
#include <sge/model/vertex_sequence.hpp>
#include <sge/model/texcoord_sequence.hpp>
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
		*model->texcoords(part_name);

	sge::model::vertex_sequence::const_iterator vit = vertices.begin();
	// Gnihihi
	sge::model::texcoord_sequence::const_iterator tit = texcoords.begin();

	FCPPT_ASSERT(
		vertices.size() == texcoords.size());

	FCPPT_ASSERT(
		vertices.size() % 3 == 0);

	for (; vit != vertices.end(); vit += 3,tit += 3)
	{
		result.triangles.push_back(
			triangle(
				{
					fcppt::math::vector::structure_cast<vec3>(
						*vit),
					fcppt::math::vector::structure_cast<vec3>(
						*(vit+1)),
					fcppt::math::vector::structure_cast<vec3>(
						*(vit+2))
				},
				{
					fcppt::math::vector::structure_cast<vec2>(
						*tit),
					fcppt::math::vector::structure_cast<vec2>(
						*(tit+1)),
					fcppt::math::vector::structure_cast<vec2>(
						*(tit+2))
				}));
	}

	return result;
}
