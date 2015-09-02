#ifndef FRUITAPP_FRUIT_CALCULATE_CUT_GEOMETRY_HPP_INCLUDED
#define FRUITAPP_FRUIT_CALCULATE_CUT_GEOMETRY_HPP_INCLUDED

#include <fruitapp/fruit/cut_geometry_fwd.hpp>
#include <fruitapp/fruit/object_fwd.hpp>
#include <fruitapp/fruit/hull/intersection_pair.hpp>
#include <sge/camera/base_fwd.hpp>
#include <sge/renderer/target/base_fwd.hpp>
#include <fcppt/preprocessor/pure.hpp>


namespace fruitapp
{
namespace fruit
{
/**
\brief Takes a two-dimensional intersection specification and turns it into a three-dimensional cut geometry
*/
fruitapp::fruit::cut_geometry
calculate_cut_geometry(
	fruitapp::fruit::hull::intersection_pair const &,
	sge::renderer::target::base const &,
	sge::camera::base const &,
	fruitapp::fruit::object const &)
FCPPT_PP_PURE;
}
}

#endif
