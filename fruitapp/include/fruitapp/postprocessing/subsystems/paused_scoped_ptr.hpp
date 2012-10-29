#ifndef FRUITAPP_POSTPROCESSING_SUBSYSTEMS_PAUSED_SCOPED_PTR_HPP_INCLUDED
#define FRUITAPP_POSTPROCESSING_SUBSYSTEMS_PAUSED_SCOPED_PTR_HPP_INCLUDED

#include <fruitapp/postprocessing/subsystems/paused_fwd.hpp>
#include <fcppt/scoped_ptr.hpp>


namespace fruitapp
{
namespace postprocessing
{
namespace subsystems
{
typedef
fcppt::scoped_ptr
<
	fruitapp::postprocessing::subsystems::paused
>
paused_scoped_ptr;
}
}
}

#endif
