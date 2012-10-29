#ifndef FRUITAPP_POSTPROCESSING_SUBSYSTEMS_MAIN_UNIQUE_PTR_HPP_INCLUDED
#define FRUITAPP_POSTPROCESSING_SUBSYSTEMS_MAIN_UNIQUE_PTR_HPP_INCLUDED

#include <fruitapp/postprocessing/subsystems/main_fwd.hpp>
#include <fcppt/unique_ptr.hpp>


namespace fruitapp
{
namespace postprocessing
{
namespace subsystems
{
typedef
fcppt::unique_ptr
<
	fruitapp::postprocessing::subsystems::main
>
main_unique_ptr;
}
}
}

#endif
