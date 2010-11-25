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
#include <iostream>

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

	std::cout << "There are " << texcoords.size() << "  texcoords and vertices\n";

	FCPPT_ASSERT(
		vertices.size() == texcoords.size());

	FCPPT_ASSERT(
		indices.size() % 3 == 0);

	std::cout << "There are " << indices.size() << " indices\n";

	for(
		sge::model::index_sequence::const_iterator index = indices.begin();
		index != indices.end();
		index += 3)
	{
		std::cerr << "assigning vertices\n";
		triangle::vertex_array vertices;
		std::cerr << "1\n";
		vertices[0] = 
			fcppt::math::vector::structure_cast<sge::renderer::vector3>(
				vertices[*index]);
		std::cerr << "2\n";
		vertices[1] = 
			fcppt::math::vector::structure_cast<sge::renderer::vector3>(
				vertices[*(index+1)]);
		std::cerr << "3\n";
		vertices[2] = 
			fcppt::math::vector::structure_cast<sge::renderer::vector3>(
				vertices[*(index+2)]);
		/*
		triangle::vertex_array const vertices = 
			{{
				fcppt::math::vector::structure_cast<sge::renderer::vector3>(
					vertices[*index]),
				fcppt::math::vector::structure_cast<sge::renderer::vector3>(
					vertices[*(index+1)]),
				fcppt::math::vector::structure_cast<sge::renderer::vector3>(
					vertices[*(index+2)])
			}};
		*/

		std::cerr << "assigning texcoords\n";
		triangle::texcoord_array texcoords;
		texcoords[0] = 
			fcppt::math::vector::structure_cast<sge::renderer::vector2>(
				texcoords[*index]);
		texcoords[1] = 
			fcppt::math::vector::structure_cast<sge::renderer::vector2>(
				texcoords[*(index+1)]);
		texcoords[2] = 
			fcppt::math::vector::structure_cast<sge::renderer::vector2>(
				texcoords[*(index+2)]);

		std::cerr << "constructing triangle\n";
		result.triangles.push_back(
			triangle(
				vertices,
				texcoords));
	}

	std::cerr << "done\n";
	return result;
}
