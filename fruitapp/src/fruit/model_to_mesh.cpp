#include <fruitapp/fruit/mesh.hpp>
#include <fruitapp/fruit/model_to_mesh.hpp>
#include <fruitapp/fruit/triangle.hpp>
#include <sge/model/md3/index_sequence.hpp>
#include <sge/model/md3/normal_sequence.hpp>
#include <sge/model/md3/object.hpp>
#include <sge/model/md3/string.hpp>
#include <sge/model/md3/texcoord_sequence.hpp>
#include <sge/renderer/vector2.hpp>
#include <sge/renderer/vector3.hpp>
#include <fcppt/make_unique_ptr_fcppt.hpp>
#include <fcppt/algorithm/array_fold.hpp>
#include <fcppt/assert/optional_error.hpp>
#include <fcppt/assert/pre_message.hpp>
#include <fcppt/cast/size_fun.hpp>
#include <fcppt/math/vector/object_impl.hpp>
#include <fcppt/math/vector/structure_cast.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/next_prior.hpp>
#include <cstddef>
#include <fcppt/config/external_end.hpp>


fruitapp::fruit::mesh_unique_ptr
fruitapp::fruit::model_to_mesh(
	sge::model::md3::object const &model)
{
	fruitapp::fruit::mesh_unique_ptr result(
		fcppt::make_unique_ptr_fcppt<fruitapp::fruit::mesh>(
			fruitapp::fruit::mesh::triangle_sequence()));

	FCPPT_ASSERT_PRE_MESSAGE(
		!model.part_names().empty(),
		FCPPT_TEXT("The model has no parts!"));

	sge::model::md3::string const part_name =
		model.part_names().front();

	sge::model::md3::vertex_sequence const vertices =
		model.vertices(
			part_name);

	sge::model::md3::texcoord_sequence const texcoords =
		FCPPT_ASSERT_OPTIONAL_ERROR(
			model.texcoords(
				part_name));

	sge::model::md3::normal_sequence const normals =
		FCPPT_ASSERT_OPTIONAL_ERROR(
			model.normals(
				part_name));

	sge::model::md3::index_sequence const indices =
		model.indices(
			part_name);

	FCPPT_ASSERT_PRE_MESSAGE(
		vertices.size() == texcoords.size() && vertices.size() == normals.size(),
		FCPPT_TEXT("Not every vertex has a normal or texture coordinate, respectively!"));

	FCPPT_ASSERT_PRE_MESSAGE(
		indices.size() % 3 == 0,
		FCPPT_TEXT("The model doesn't consist solely of triangles"));

	for(
		sge::model::md3::index_sequence::const_iterator index = indices.begin();
		index != indices.end();
		index += 3)
	{
		result->triangles().push_back(
			fruitapp::fruit::triangle(
				fcppt::algorithm::array_fold<
					fruitapp::fruit::triangle::vertex_array
				>(
					[
						index,
						&vertices
					](
						std::size_t const _array_index
					)
					{
						return
							fcppt::math::vector::structure_cast<
								fruitapp::fruit::triangle::vector,
								fcppt::cast::size_fun
							>(
								vertices[
									*boost::next(
										index,
										_array_index
									)
								]
							);
					}
				),
				fcppt::algorithm::array_fold<
					fruitapp::fruit::triangle::texcoord_array
				>(
					[
						index,
						&texcoords
					](
						std::size_t const _array_index
					)
					{
						return
							fcppt::math::vector::structure_cast<
								sge::renderer::vector2,
								fcppt::cast::size_fun
							>(
								texcoords[
									*boost::next(
										index,
										_array_index
									)
								]
							);
					}
				),
				fcppt::algorithm::array_fold<
					fruitapp::fruit::triangle::normal_array
				>(
					[
						index,
						&normals
					](
						std::size_t const _array_index
					)
					{
						return
							fcppt::math::vector::structure_cast<
								fruitapp::fruit::triangle::vector,
								fcppt::cast::size_fun
							>(
								normals[
									*boost::next(
										index,
										_array_index
									)
								]
							);
					}
				)
			)
		);
	}

	return
		result;
}
