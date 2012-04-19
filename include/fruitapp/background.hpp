#ifndef FRUITAPP_BACKGROUND_HPP_INCLUDED
#define FRUITAPP_BACKGROUND_HPP_INCLUDED

#include <fruitapp/shadow_map_texture.hpp>
#include <fruitapp/shadow_mvp.hpp>
#include <fruitapp/projection_manager/projection_change.hpp>
#include <fruitlib/scenic/node.hpp>
#include <fruitlib/scenic/optional_parent.hpp>
#include <fruitlib/scenic/events/render.hpp>
#include <fruitlib/scenic/events/viewport_change.hpp>
#include <sge/camera/base_fwd.hpp>
#include <sge/image2d/system_fwd.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <sge/renderer/device_fwd.hpp>
#include <sge/renderer/matrix4.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/renderer/vertex_buffer_scoped_ptr.hpp>
#include <sge/renderer/vertex_declaration_scoped_ptr.hpp>
#include <sge/renderer/texture/planar_shared_ptr.hpp>
#include <sge/shader/object.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
class background
:
	public fruitlib::scenic::node<background>
{
FCPPT_NONCOPYABLE(
	background);
public:
	typedef
	boost::mpl::vector2
	<
		fruitlib::scenic::events::render,
		fruitapp::projection_manager::projection_change
	>
	scene_reactions;

	explicit
	background(
		fruitlib::scenic::optional_parent const &,
		sge::renderer::device &,
		sge::image2d::system &,
		fruitapp::shadow_mvp const &,
		fruitapp::shadow_map_texture const &,
		sge::parse::json::object const &,
		sge::camera::base const &);

	~background();

	void
	react(
		fruitlib::scenic::events::render const &);

	void
	react(
		fruitapp::projection_manager::projection_change const &);
private:
	sge::renderer::device &renderer_;
	sge::camera::base const &camera_;
	sge::renderer::texture::planar_shared_ptr texture_;
	sge::renderer::vertex_declaration_scoped_ptr vertex_declaration_;
	sge::renderer::vertex_buffer_scoped_ptr vb_;
	sge::shader::object shader_;
	sge::renderer::scalar const reps_;

};
}

#endif
