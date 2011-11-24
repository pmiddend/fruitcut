#ifndef FRUITAPP_BACKGROUND_HPP_INCLUDED
#define FRUITAPP_BACKGROUND_HPP_INCLUDED

#include <fruitlib/scenic/node.hpp>
#include <fruitlib/scenic/optional_parent.hpp>
#include <fruitlib/scenic/events/render.hpp>
#include <fruitlib/scenic/events/viewport_change.hpp>
#include <sge/camera/first_person/object_fwd.hpp>
#include <sge/image2d/system_fwd.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <sge/renderer/device_fwd.hpp>
#include <sge/renderer/matrix4.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/renderer/vertex_buffer_ptr.hpp>
#include <sge/renderer/vertex_declaration_ptr.hpp>
#include <sge/renderer/texture/planar_ptr.hpp>
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
	boost::mpl::vector2<fruitlib::scenic::events::render,fruitlib::scenic::events::viewport_change>
	scene_reactions;

	explicit
	background(
		fruitlib::scenic::optional_parent const &,
		sge::renderer::device &,
		sge::image2d::system &,
		sge::renderer::texture::planar_ptr,
		sge::renderer::matrix4 const &,
		sge::parse::json::object const &,
		sge::camera::first_person::object const &);

	~background();

	void
	react(
		fruitlib::scenic::events::render const &);

	void
	react(
		fruitlib::scenic::events::viewport_change const &);
private:
	sge::renderer::device &renderer_;
	sge::camera::first_person::object const &camera_;
	sge::renderer::texture::planar_ptr texture_;
	sge::renderer::vertex_declaration_ptr vertex_declaration_;
	sge::renderer::vertex_buffer_ptr vb_;
	sge::shader::object shader_;
	sge::renderer::scalar const reps_;

};
}

#endif
