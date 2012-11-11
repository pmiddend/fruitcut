#ifndef FRUITAPP_FRUIT_CUT_CONTEXT_HPP_INCLUDED
#define FRUITAPP_FRUIT_CUT_CONTEXT_HPP_INCLUDED

#include <fruitapp/fruit/area.hpp>
#include <fruitapp/fruit/cut_geometry.hpp>
#include <fruitapp/fruit/mesh.hpp>
#include <fruitapp/fruit/mesh_scoped_ptr.hpp>
#include <fruitapp/fruit/mesh_unique_ptr.hpp>
#include <fruitapp/fruit/object_fwd.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/container/array.hpp>
#include <fcppt/preprocessor/const.hpp>
#include <fcppt/preprocessor/pure.hpp>


namespace fruitapp
{
namespace fruit
{
/**
\brief Represents a three-dimensional cut of a fruit into two halves

Note that there is cut_context and cut_mesh_result. The latter is more
"low-level", dealing with a single cut operation whereas cut_context
deals with the fruit being cut at _both_ planes.
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

	cut_context(
		fruitapp::fruit::object const &_old,
		new_fruit_array const &,
		fruitapp::fruit::area const &,
		fruitapp::fruit::cut_geometry const &,
		fruitapp::fruit::mesh_unique_ptr);

	fruitapp::fruit::object const &
	old() const
	FCPPT_PP_CONST;

	new_fruit_array const &
	new_fruits() const
	FCPPT_PP_CONST;

	fruitapp::fruit::area const
	area() const
	FCPPT_PP_PURE;

	fruitapp::fruit::cut_geometry const &
	cut_geometry() const
	FCPPT_PP_CONST;

	fruitapp::fruit::mesh const &
	cross_section() const
	FCPPT_PP_CONST;

	~cut_context();
private:
	fruitapp::fruit::object const &old_;
	new_fruit_array new_;
	fruitapp::fruit::area const area_;
	fruitapp::fruit::cut_geometry const cut_geometry_;
	fruitapp::fruit::mesh_scoped_ptr const cross_section_;
};
}
}

#endif
