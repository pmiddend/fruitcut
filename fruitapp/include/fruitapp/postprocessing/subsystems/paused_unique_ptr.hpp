#ifndef FRUITAPP_POSTPROCESSING_SUBSYSTEMS_PAUSED_UNIQUE_PTR_HPP_INCLUDED
#define FRUITAPP_POSTPROCESSING_SUBSYSTEMS_PAUSED_UNIQUE_PTR_HPP_INCLUDED

#include <fruitapp/postprocessing/subsystems/paused_fwd.hpp>
#include <fcppt/config/external_begin.hpp>
#include <memory>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
namespace postprocessing
{
namespace subsystems
{
typedef
std::unique_ptr
<
	fruitapp::postprocessing::subsystems::paused
>
paused_unique_ptr;
}
}
}

#endif
