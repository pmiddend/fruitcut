#ifndef FRUITAPP_VIEWPORT_OPTIONAL_HPP_INCLUDED
#define FRUITAPP_VIEWPORT_OPTIONAL_HPP_INCLUDED

#include <sge/renderer/target/viewport.hpp>
#include <fcppt/optional/object_fwd.hpp>


namespace fruitapp
{
namespace viewport
{
typedef
fcppt::optional::object<sge::renderer::target::viewport>
optional;
}
}

#endif

