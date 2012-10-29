#ifndef FRUITAPP_POSTPROCESSING_RENDER_CALLBACK_HPP_INCLUDED
#define FRUITAPP_POSTPROCESSING_RENDER_CALLBACK_HPP_INCLUDED

#include <sge/renderer/context/core_fwd.hpp>
#include <fcppt/function/object.hpp>


namespace fruitapp
{
namespace postprocessing
{
typedef
fcppt::function::object<void (sge::renderer::context::core &)>
render_callback;
}
}

#endif
