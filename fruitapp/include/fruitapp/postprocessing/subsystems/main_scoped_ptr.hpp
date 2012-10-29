#ifndef FRUITAPP_POSTPROCESSING_SUBSYSTEMS_MAIN_SCOPED_PTR_HPP_INCLUDED
#define FRUITAPP_POSTPROCESSING_SUBSYSTEMS_MAIN_SCOPED_PTR_HPP_INCLUDED

#include <fruitapp/postprocessing/subsystems/main_fwd.hpp>
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
	fruitapp::postprocessing::subsystems::main
>
main_scoped_ptr;
}
}
}

#endif
