#ifndef FRUITLIB_PP_FILTER_MANAGER_HPP_INCLUDED
#define FRUITLIB_PP_FILTER_MANAGER_HPP_INCLUDED

#include <fruitlib/pp/screen_vf/quad.hpp>
#include <sge/renderer/device_fwd.hpp>
#include <sge/shader/scoped.hpp>
#include <sge/shader/variable_sequence.hpp>
#include <sge/shader/sampler_sequence.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/tr1/unordered_map.hpp>
#include <fcppt/filesystem/path.hpp>

namespace fruitlib
{
namespace pp
{
namespace filter
{
class manager
{
FCPPT_NONCOPYABLE(
	manager);
public:
	explicit
	manager(
		sge::renderer::device &,
		fcppt::filesystem::path const &);

	sge::shader::object &
	lookup_shader(
		fcppt::string const &,
		sge::shader::variable_sequence const &,
		sge::shader::sampler_sequence const &);

	screen_vf::quad &
	quad();

	screen_vf::quad const &
	quad() const;

	~manager();
private:
	typedef
	fcppt::shared_ptr<sge::shader::object>
	shader_ptr;

	typedef
	std::tr1::unordered_map
	<
		fcppt::string,
		shader_ptr
	>
	name_to_shader;

	sge::renderer::device &renderer_;
	fcppt::filesystem::path const base_path_;
	screen_vf::quad quad_;
	name_to_shader name_to_shader_;

	name_to_shader::iterator const
	create_shader(
		fcppt::string const &,
		sge::shader::variable_sequence const &,
		sge::shader::sampler_sequence const &);
};
}
}
}

#endif
