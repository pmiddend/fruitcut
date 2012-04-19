#include <fruitlib/perspective_projection_information.hpp>
#include <fruitlib/perspective_projection_information_to_matrix.hpp>
#include <sge/renderer/projection/perspective_af.hpp>
#include <fcppt/math/matrix/object_impl.hpp>


sge::renderer::matrix4 const
fruitlib::perspective_projection_information_to_matrix(
	fruitlib::perspective_projection_information const &_information)
{
	return
		sge::renderer::projection::perspective_af(
				_information.aspect(),
				_information.fov(),
				_information.near(),
				_information.far());
}
