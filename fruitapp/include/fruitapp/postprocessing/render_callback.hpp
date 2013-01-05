#ifndef FRUITAPP_POSTPROCESSING_RENDER_CALLBACK_HPP_INCLUDED
#define FRUITAPP_POSTPROCESSING_RENDER_CALLBACK_HPP_INCLUDED

#include <sge/renderer/context/core_fwd.hpp>
#include <fcppt/config/external_begin.hpp>
#include <functional>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
namespace postprocessing
{
typedef
std::function<void (sge::renderer::context::core &)>
render_callback;
}
}

#endif
