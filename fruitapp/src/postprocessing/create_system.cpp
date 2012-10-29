#include <fruitapp/postprocessing/create_system.hpp>
#include <fruitapp/postprocessing/cg/system.hpp>
#include <fcppt/cref.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/ref.hpp>


fruitapp::postprocessing::system_unique_ptr
fruitapp::postprocessing::create_system(
	sge::shader::context &_shader_context,
	fruitapp::viewport::manager &_viewport_manager,
	sge::parse::json::object const &_object)
{
	return
		fruitapp::postprocessing::system_unique_ptr(
			fcppt::make_unique_ptr<fruitapp::postprocessing::cg::system>(
				fcppt::ref(
					_shader_context),
				fcppt::ref(
					_viewport_manager),
				fcppt::cref(
					_object)));
}
