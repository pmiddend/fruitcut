#ifndef FRUITAPP_POSTPROCESSING_SYSTEM_UNIQUE_PTR_HPP_INCLUDED
#define FRUITAPP_POSTPROCESSING_SYSTEM_UNIQUE_PTR_HPP_INCLUDED

#include <fruitapp/postprocessing/system_fwd.hpp>
#include <fcppt/unique_ptr_impl.hpp>


namespace fruitapp
{
namespace postprocessing
{
typedef
fcppt::unique_ptr<fruitapp::postprocessing::system>
system_unique_ptr;
}
}

#endif
