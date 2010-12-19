#ifndef FRUITCUT_SANDBOX_PP_SCENE_RENDER_CALLBACK_HPP_INCLUDED
#define FRUITCUT_SANDBOX_PP_SCENE_RENDER_CALLBACK_HPP_INCLUDED

#include <boost/function.hpp>

namespace fruitcut
{
namespace sandbox
{
namespace pp
{
typedef
boost::function<void ()>
scene_render_callback;
}
}
}

#endif
