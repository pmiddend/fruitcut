#ifndef FRUITAPP_POSTPROCESSING_CREATE_SYSTEM_HPP_INCLUDED
#define FRUITAPP_POSTPROCESSING_CREATE_SYSTEM_HPP_INCLUDED

#include <fruitapp/postprocessing/system_unique_ptr.hpp>
#include <fruitapp/viewport/manager_fwd.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <sge/shader/context_fwd.hpp>


namespace fruitapp
{
namespace postprocessing
{
fruitapp::postprocessing::system_unique_ptr
create_system(
	sge::shader::context &,
	fruitapp::viewport::manager &,
	sge::parse::json::object const &);
}
}

#endif
