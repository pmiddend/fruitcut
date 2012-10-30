#ifndef FRUITAPP_FRUIT_RENDERING_SHADOW_HPP_INCLUDED
#define FRUITAPP_FRUIT_RENDERING_SHADOW_HPP_INCLUDED

#include <fruitapp/fruit/manager_fwd.hpp>
#include <fruitapp/shadow_map/mvp.hpp>
#include <sge/camera/first_person/object_fwd.hpp>
#include <sge/renderer/matrix4.hpp>
#include <sge/renderer/state/core/depth_stencil/object_scoped_ptr.hpp>
#include <sge/shader/context_fwd.hpp>
#include <sge/shader/pair.hpp>
#include <sge/shader/parameter/matrix.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/math/matrix/object_impl.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <fcppt/config/external_end.hpp>

namespace fruitapp
{
namespace fruit
{
namespace rendering
{
class shadow
{
FCPPT_NONCOPYABLE(
	shadow);
public:
	shadow(
		sge::shader::context &,
		fruitapp::fruit::manager const &,
		fruitapp::shadow_map::mvp const &);

	~shadow();

	void
	render(
		sge::renderer::context::core &);
private:
	fruitapp::fruit::manager const &manager_;
	sge::shader::pair shader_;
	sge::shader::parameter::matrix<sge::renderer::scalar,4,4> mvp_parameter_;
	fruitapp::shadow_map::mvp const mvp_;
	sge::renderer::state::core::depth_stencil::object_scoped_ptr const depth_stencil_state_;
};
}
}
}

#endif
