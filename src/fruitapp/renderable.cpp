#include <fruitapp/renderable.hpp>
#include <fruitlib/scenic/depth.hpp>
#include <fruitlib/scenic/parent.hpp>
#include <sge/parse/json/find_and_convert_member.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <sge/parse/json/path.hpp>
#include <sge/systems/instance.hpp>
#include <fcppt/text.hpp>
#include <fcppt/tr1/functional.hpp>


fruitapp::renderable::renderable(
	fruitlib::scenic::optional_parent const &_parent,
	sge::systems::instance const &_systems,
	sge::parse::json::object const &_config_file)
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
		_systems.renderer(),
		std::tr1::bind(
			&scene::render_children,
			&scene_),
		sge::parse::json::find_and_convert_member<sge::parse::json::object>(
			_config_file,
			sge::parse::json::path(
				FCPPT_TEXT("pp")))),
	overlay_(
		fruitlib::scenic::optional_parent(
			fruitlib::scenic::parent(
				*this,
				fruitlib::scenic::depth(
					2))),
		_systems.renderer(),
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
