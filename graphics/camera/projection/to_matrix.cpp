#include "to_matrix.hpp"
#include "../../../scalar.hpp"
#include <fcppt/math/matrix/orthogonal_xy.hpp>
#include <fcppt/math/matrix/perspective.hpp>
#include <fcppt/variant/apply_unary.hpp>

namespace
{
class conversion_operator
{
public:
	typedef 
	fruitcut::mat4 
	result_type;

	fruitcut::mat4 const
	operator()(fruitcut::graphics::camera::projection::orthogonal const &o) const
	{
		return fcppt::math::matrix::orthogonal_xy<fruitcut::scalar>();
	}

	fruitcut::mat4 const
	operator()(fruitcut::graphics::camera::projection::perspective const &o) const
	{
		return 
			fcppt::math::matrix::perspective(
				o.aspect(),
				o.fov(),
				o.near(),
				o.far());
	}
};
}

fruitcut::mat4 const
fruitcut::graphics::camera::projection::to_matrix(
	object const &o)
{
	return 
		fcppt::variant::apply_unary(
			conversion_operator(),
			o);
}
