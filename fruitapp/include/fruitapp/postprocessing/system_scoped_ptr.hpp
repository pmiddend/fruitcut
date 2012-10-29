#ifndef FRUITAPP_POSTPROCESSING_SYSTEM_SCOPED_PTR_HPP_INCLUDED
#define FRUITAPP_POSTPROCESSING_SYSTEM_SCOPED_PTR_HPP_INCLUDED

#include <fruitapp/postprocessing/system_fwd.hpp>
#include <fcppt/scoped_ptr.hpp>

namespace fruitapp
{
namespace postprocessing
{
typedef
fcppt::scoped_ptr<fruitapp::postprocessing::system>
system_scoped_ptr;
}
}

#endif
