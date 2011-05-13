#ifndef FRUITCUT_APP_FRUIT_CUT_CONTEXT_HPP_INCLUDED
#define FRUITCUT_APP_FRUIT_CUT_CONTEXT_HPP_INCLUDED

#include "object_fwd.hpp"
#include "area.hpp"
#include "../../physics/vector3.hpp"
#include <fcppt/container/array.hpp>
#include <fcppt/reference_wrapper.hpp>

namespace fruitcut
{
namespace app
{
namespace fruit
{
class cut_context
{
public:
	typedef
	fruit::object const *
	fruit_ptr;

	typedef
	fcppt::container::array<fruit_ptr,2>
	new_fruit_array;

	explicit
	cut_context(
		fruit::object const &_old,
		new_fruit_array const &,
		fruit::area,
		physics::vector3 const &cut_direction);

	fruit::object const &
	old() const;

	new_fruit_array const &
	new_fruits() const;

	fruit::area
	area() const;

	physics::vector3 const &
	cut_direction() const;
private:
	fruit_ptr old_;
	new_fruit_array new_;
	fruit::area area_;
	physics::vector3 cut_direction_;
};
}
}
}

#endif
