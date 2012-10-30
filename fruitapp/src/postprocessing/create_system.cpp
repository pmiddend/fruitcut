#include <fruitapp/postprocessing/create_system.hpp>
#include <fruitapp/postprocessing/cg/system.hpp>
#include <fruitapp/postprocessing/dummy/system.hpp>
#include <sge/parse/json/find_and_convert_member.hpp>
#include <sge/parse/json/string_to_path.hpp>
#include <fcppt/cref.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/ref.hpp>
#include <fcppt/text.hpp>


fruitapp::postprocessing::system_unique_ptr
fruitapp::postprocessing::create_system(
	sge::shader::optional_context_ref const &_shader_context,
	fruitapp::viewport::manager &_viewport_manager,
	sge::parse::json::object const &_object)
{
	return
		_shader_context.has_value() &&
		sge::parse::json::find_and_convert_member<bool>(
			_object,
			sge::parse::json::string_to_path(
				FCPPT_TEXT("graphics/use-postprocessing")))
		?
			fruitapp::postprocessing::system_unique_ptr(
				fcppt::make_unique_ptr<fruitapp::postprocessing::cg::system>(
					fcppt::ref(
						*_shader_context),
					fcppt::ref(
						_viewport_manager),
					fcppt::cref(
						sge::parse::json::find_and_convert_member<sge::parse::json::object const>(
							_object,
							sge::parse::json::path(
								FCPPT_TEXT("pp"))))))
		:
			fruitapp::postprocessing::system_unique_ptr(
				fcppt::make_unique_ptr<fruitapp::postprocessing::dummy::system>());
}
