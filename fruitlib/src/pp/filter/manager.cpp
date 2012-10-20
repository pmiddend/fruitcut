#include <fruitlib/pp/filter/manager.hpp>
#include <sge/shader/context.hpp>

fruitlib::pp::filter::manager::manager(
	sge::shader::context &_shader_context,
	fruitlib::pp::filter::base_path const &_base_path)
:
	shader_context_(
		_shader_context),
	base_path_(
		_base_path),
	quad_(
		this->renderer())
{

}

fruitlib::pp::screen_vf::quad &
fruitlib::pp::filter::manager::quad()
{
	return quad_;
}

fruitlib::pp::screen_vf::quad const &
fruitlib::pp::filter::manager::quad() const
{
	return quad_;
}

fruitlib::pp::filter::manager::~manager()
{
}

sge::renderer::device::core &
fruitlib::pp::filter::manager::renderer() const
{
	return
		this->shader_context().renderer();
}

sge::shader::context &
fruitlib::pp::filter::manager::shader_context() const
{
	return
		shader_context_;
}

sge::shader::optional_cflags const
fruitlib::pp::filter::manager::shader_cflags() const
{
	return
		sge::shader::optional_cflags();
}

fruitlib::pp::filter::base_path const &
fruitlib::pp::filter::manager::base_path() const
{
	return
		base_path_;
}
