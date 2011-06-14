#include "manager.hpp"
#include "../screen_vf/format.hpp"
#include <sge/shader/shader.hpp>
#include <fcppt/make_shared_ptr.hpp>

fruitcut::fruitlib::pp::filter::manager::manager(
	sge::renderer::device &_renderer,
	fcppt::filesystem::path const &_base_path)
:
	renderer_(
		_renderer),
	base_path_(
		_base_path),
	quad_(
		renderer_),
	name_to_shader_()
{
	
}

sge::shader::object &
fruitcut::fruitlib::pp::filter::manager::lookup_shader(
	fcppt::string const &name,
	sge::shader::variable_sequence const &variables,
	sge::shader::sampler_sequence const &samplers)
{
	name_to_shader::iterator found_shader = 
		name_to_shader_.find(
			name);

	if(found_shader == name_to_shader_.end())
		found_shader = 
			create_shader(
				name,
				variables,
				samplers);

	return 
		*(found_shader->second);
}

fruitcut::fruitlib::pp::screen_vf::quad &
fruitcut::fruitlib::pp::filter::manager::quad()
{
	return quad_;
}

fruitcut::fruitlib::pp::screen_vf::quad const &
fruitcut::fruitlib::pp::filter::manager::quad() const
{
	return quad_;
}

fruitcut::fruitlib::pp::filter::manager::~manager()
{
}

fruitcut::fruitlib::pp::filter::manager::name_to_shader::iterator const
fruitcut::fruitlib::pp::filter::manager::create_shader(
	fcppt::string const &name,
	sge::shader::variable_sequence const &variables,
	sge::shader::sampler_sequence const &samplers)
{
	return 
		name_to_shader_.insert(
			name_to_shader::value_type(
				name,
				fcppt::make_shared_ptr<sge::shader::object>(
					sge::shader::object_parameters(
						renderer_,
						quad_.vertex_declaration(),
						base_path_/name/FCPPT_TEXT("vertex.glsl"),
						base_path_/name/FCPPT_TEXT("fragment.glsl"),
						sge::shader::vf_to_string<screen_vf::format>(),
						variables,
						samplers)))).first;
}