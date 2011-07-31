#ifndef FRUITAPP_FRUIT_CUT_CONTEXT_HPP_INCLUDED
#define FRUITAPP_FRUIT_CUT_CONTEXT_HPP_INCLUDED

#include "object_fwd.hpp"
#include "area.hpp"
#include "mesh.hpp"
#include <fruitlib/physics/vector3.hpp>
#include <fcppt/container/array.hpp>
#include <fcppt/reference_wrapper.hpp>

namespace fruitapp
{
namespace fruit
{
/**
	Note that there is cut_context and cut_mesh_result. The latter is
	more "low-level", dealing with a single cut operation whereas
	cut_context deals with the fruit being cut at _both_ planes.
 */
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
		fruit::area const &,
		fruitlib::physics::vector3 const &cut_direction,
		fruit::mesh const &);

	fruit::object const &
	old() const;

	new_fruit_array const &
	new_fruits() const;

	fruit::area::value_type
	area() const;

	fruitlib::physics::vector3 const &
	cut_direction() const;

	fruit::mesh const &
	cross_section() const;
private:
	fruit_ptr old_;
	new_fruit_array new_;
	fruit::area::value_type area_;
	fruitlib::physics::vector3 cut_direction_;
	fruit::mesh cross_section_;
};
}
}

#endif
