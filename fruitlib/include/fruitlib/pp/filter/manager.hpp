#ifndef FRUITLIB_PP_FILTER_MANAGER_HPP_INCLUDED
#define FRUITLIB_PP_FILTER_MANAGER_HPP_INCLUDED

#include <fruitlib/pp/screen_vf/quad.hpp>
#include <sge/renderer/device/core_fwd.hpp>
#include <sge/shader/context_fwd.hpp>
#include <sge/shader/optional_cflags.hpp>
#include <fcppt/noncopyable.hpp>


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
		sge::shader::context &);

	fruitlib::pp::screen_vf::quad &
	quad();

	fruitlib::pp::screen_vf::quad const &
	quad() const;

	sge::renderer::device::core &
	renderer() const;

	sge::shader::context &
	shader_context() const;

	sge::shader::optional_cflags const
	shader_cflags() const;

	~manager();
private:
	sge::shader::context &shader_context_;
	fruitlib::pp::screen_vf::quad quad_;
};
}
}
}

#endif
