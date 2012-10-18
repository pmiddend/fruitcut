#include <fruitapp/renderable.hpp>
#include <fruitlib/scenic/depth.hpp>
#include <fruitlib/scenic/parent.hpp>
#include <sge/parse/json/find_and_convert_member.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <sge/parse/json/path.hpp>
#include <sge/renderer/device/ffp.hpp>
#include <sge/shader/context.hpp>
#include <sge/systems/instance.hpp>
#include <fcppt/text.hpp>
#include <fcppt/tr1/functional.hpp>


fruitapp::renderable::renderable(
	fruitlib::scenic::optional_parent const &_parent,
	sge::shader::context &_shader_context,
	sge::parse::json::object const &_config_file,
	fruitapp::viewport::manager &_viewport_manager)
:
	node_base(
		_parent),
	scene_(
		fruitlib::scenic::optional_parent(
			fruitlib::scenic::parent(
				*this,
				fruitlib::scenic::depth(
					0)))),
	postprocessing_(
		fruitlib::scenic::optional_parent(
			fruitlib::scenic::parent(
				*this,
				fruitlib::scenic::depth(
					1))),
		_shader_context,
		std::tr1::bind(
			&fruitapp::scene::render_children,
			&scene_,
			std::tr1::placeholders::_1),
		sge::parse::json::find_and_convert_member<sge::parse::json::object>(
			_config_file,
			sge::parse::json::path(
				FCPPT_TEXT("pp"))),
		_viewport_manager),
	overlay_(
		fruitlib::scenic::optional_parent(
			fruitlib::scenic::parent(
				*this,
				fruitlib::scenic::depth(
					2))),
		dynamic_cast<sge::renderer::device::ffp &>(
			_shader_context.renderer()),
		postprocessing_)
{
}

fruitapp::scene &
fruitapp::renderable::scene()
{
	return scene_;
}

fruitapp::scene const &
fruitapp::renderable::scene() const
{
	return scene_;
}

fruitapp::overlay &
fruitapp::renderable::overlay()
{
	return overlay_;
}

fruitapp::overlay const &
fruitapp::renderable::overlay() const
{
	return overlay_;
}

fruitapp::postprocessing &
fruitapp::renderable::postprocessing()
{
	return postprocessing_;
}

fruitapp::postprocessing const &
fruitapp::renderable::postprocessing() const
{
	return postprocessing_;
}

fruitapp::renderable::~renderable()
{
}
