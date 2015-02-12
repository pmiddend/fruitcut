#include <fruitapp/graphics_settings/object.hpp>
#include <fruitapp/postprocessing/create_system.hpp>
#include <fruitapp/postprocessing/cg/system.hpp>
#include <fruitapp/postprocessing/dummy/system.hpp>
#include <sge/parse/json/find_and_convert_member.hpp>
#include <sge/parse/json/path.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/text.hpp>


fruitapp::postprocessing::system_unique_ptr
fruitapp::postprocessing::create_system(
	sge::shader::optional_context_ref const &_shader_context,
	fruitapp::viewport::manager &_viewport_manager,
	fruitapp::graphics_settings::object const &_graphics_settings,
	sge::parse::json::object const &_object)
{
	return
		_shader_context.has_value() &&
		sge::parse::json::find_and_convert_member<bool>(
			_graphics_settings.current(),
			sge::parse::json::path(
				FCPPT_TEXT("use-postprocessing")))
		?
			fruitapp::postprocessing::system_unique_ptr(
				fcppt::make_unique_ptr<fruitapp::postprocessing::cg::system>(
					*_shader_context,
					_viewport_manager,
					sge::parse::json::find_and_convert_member<sge::parse::json::object const>(
						_object,
						sge::parse::json::path(
							FCPPT_TEXT("pp")))))
		:
			fruitapp::postprocessing::system_unique_ptr(
				fcppt::make_unique_ptr<fruitapp::postprocessing::dummy::system>());
}
