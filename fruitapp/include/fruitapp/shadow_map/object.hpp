#ifndef FRUITAPP_SHADOW_MAP_OBJECT_HPP_INCLUDED
#define FRUITAPP_SHADOW_MAP_OBJECT_HPP_INCLUDED

#include <fruitapp/light/manager_fwd.hpp>
#include <fruitapp/shadow_map/mvp.hpp>
#include <fruitlib/scenic/node.hpp>
#include <fruitlib/scenic/optional_parent.hpp>
#include <fruitlib/scenic/events/update.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <sge/renderer/matrix4.hpp>
#include <sge/renderer/device/ffp_fwd.hpp>
#include <sge/renderer/state/core/depth_stencil/object_unique_ptr.hpp>
#include <sge/renderer/target/offscreen_unique_ptr.hpp>
#include <sge/renderer/texture/planar_unique_ptr.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/math/matrix/object_impl.hpp>
#include <fcppt/preprocessor/pure.hpp>
#include <fcppt/preprocessor/warn_unused_result.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
namespace shadow_map
{
class object
:
	public fruitlib::scenic::node<object>
{
FCPPT_NONCOPYABLE(
	object);
public:
	typedef
	boost::mpl::vector1<fruitlib::scenic::events::update>
	scene_reactions;

	object(
		fruitlib::scenic::optional_parent const &,
		sge::parse::json::object const &,
		sge::renderer::device::ffp &,
		fruitapp::light::manager const &);

	sge::renderer::texture::planar &
	texture();

	fruitapp::shadow_map::mvp const
	mvp() const;

	~object();

	void
	react(
		fruitlib::scenic::events::update const &);
private:
	sge::renderer::device::ffp &renderer_;
	sge::renderer::state::core::depth_stencil::object_unique_ptr const depth_stencil_state_;
	sge::renderer::texture::planar_unique_ptr const texture_;
	sge::renderer::target::offscreen_unique_ptr const target_;
	fruitapp::shadow_map::mvp const mvp_;
};
}
}

#endif
