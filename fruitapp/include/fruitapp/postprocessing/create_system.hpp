#ifndef FRUITAPP_POSTPROCESSING_CREATE_SYSTEM_HPP_INCLUDED
#define FRUITAPP_POSTPROCESSING_CREATE_SYSTEM_HPP_INCLUDED

#include <fruitapp/graphics_settings/object_fwd.hpp>
#include <fruitapp/postprocessing/system_unique_ptr.hpp>
#include <fruitapp/viewport/manager_fwd.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <sge/shader/optional_context_ref.hpp>


namespace fruitapp
{
namespace postprocessing
{
fruitapp::postprocessing::system_unique_ptr
create_system(
	sge::shader::optional_context_ref const &,
	fruitapp::viewport::manager &,
	fruitapp::graphics_settings::object const &,
	sge::parse::json::object const &);
}
}

#endif
