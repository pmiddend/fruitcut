#ifndef FRUITLIB_PHYSICS_RIGID_BODY_OPTIONAL_MASS_HPP_INCLUDED
#define FRUITLIB_PHYSICS_RIGID_BODY_OPTIONAL_MASS_HPP_INCLUDED

#include <fruitlib/physics/rigid_body/mass.hpp>
#include <fcppt/optional/object.hpp>


namespace fruitlib
{
namespace physics
{
namespace rigid_body
{
typedef
fcppt::optional::object<rigid_body::mass>
optional_mass;
}
}
}

#endif
