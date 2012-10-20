#ifndef FRUITAPP_FRUIT_CUT_CONTEXT_HPP_INCLUDED
#define FRUITAPP_FRUIT_CUT_CONTEXT_HPP_INCLUDED

#include <fruitapp/fruit/area.hpp>
#include <fruitapp/fruit/mesh.hpp>
#include <fruitapp/fruit/mesh_scoped_ptr.hpp>
#include <fruitapp/fruit/mesh_unique_ptr.hpp>
#include <fruitapp/fruit/object_fwd.hpp>
#include <fruitlib/physics/vector3.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/reference_wrapper.hpp>
#include <fcppt/container/array.hpp>
#include <fcppt/preprocessor/const.hpp>
#include <fcppt/preprocessor/pure.hpp>


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
FCPPT_NONCOPYABLE(
	cut_context);
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
		fruit::mesh_unique_ptr);

	fruit::object const &
	old() const
	FCPPT_PP_CONST;

	new_fruit_array const &
	new_fruits() const
	FCPPT_PP_CONST;

	fruit::area::value_type
	area() const
	FCPPT_PP_PURE;

	fruitlib::physics::vector3 const &
	cut_direction() const
	FCPPT_PP_CONST;

	fruit::mesh const &
	cross_section() const
	FCPPT_PP_CONST;

	~cut_context();
private:
	fruit::object const &old_;
	new_fruit_array new_;
	fruit::area::value_type area_;
	fruitlib::physics::vector3 cut_direction_;
	fruit::mesh_scoped_ptr cross_section_;
};
}
}

#endif
