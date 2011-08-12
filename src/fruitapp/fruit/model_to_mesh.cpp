#include <fruitapp/fruit/model_to_mesh.hpp>
#include <fruitapp/fruit/triangle.hpp>
#include <sge/renderer/vector2.hpp>
#include <sge/renderer/vector3.hpp>
#include <sge/model/md3/normal_sequence.hpp>
#include <sge/model/md3/index_sequence.hpp>
#include <sge/model/md3/texcoord_sequence.hpp>
#include <sge/model/md3/object.hpp>
#include <fcppt/math/vector/structure_cast.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/assert.hpp>
#include <fcppt/string.hpp>
#include <boost/next_prior.hpp>

fruitapp::fruit::mesh const
fruitapp::fruit::model_to_mesh(
	sge::model::md3::object const &model)
{
	mesh result;

	FCPPT_ASSERT(
		!model.part_names().empty());

	fcppt::string const part_name =
		model.part_names().front();

	sge::model::md3::vertex_sequence const vertices =
		model.vertices(
			part_name);

	FCPPT_ASSERT(
		model.texcoords(
			part_name));

	sge::model::md3::texcoord_sequence const texcoords =
		*model.texcoords(
			part_name);

	FCPPT_ASSERT(
		model.normals(
			part_name));

	sge::model::md3::normal_sequence const normals =
		*model.normals(
			part_name);

	sge::model::md3::index_sequence const indices =
		model.indices(
			part_name);

	FCPPT_ASSERT(
		vertices.size() == texcoords.size() && vertices.size() == normals.size());

	FCPPT_ASSERT(
		indices.size() % 3 == 0);

	for(
		sge::model::md3::index_sequence::const_iterator index = indices.begin();
		index != indices.end();
		index += 3)
	{
		triangle::vertex_array vt;
		triangle::texcoord_array tc;
		triangle::normal_array ns;
		for (triangle::vertex_array::size_type i = 0; i < vt.size(); ++i)
		{
			vt[i] =
				fcppt::math::vector::structure_cast<triangle::vector>(
					vertices[
						*boost::next(
							index,
							i)]);

			ns[i] =
				fcppt::math::vector::structure_cast<triangle::vector>(
					normals[
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
				tc,
				ns));
	}

	return result;
}
