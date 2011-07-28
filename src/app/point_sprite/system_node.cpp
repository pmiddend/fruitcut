#include "system_node.hpp"
#include "../exception.hpp"
#include "../../media_path.hpp"
#include "../../fruitlib/uniform_random.hpp"
#include "../../fruitlib/resource_tree/from_directory_tree.hpp"
#include "../../fruitlib/resource_tree/navigate_to_path.hpp"
#include "../../fruitlib/resource_tree/path.hpp"
#include <sge/camera/object.hpp>
#include <sge/image2d/file.hpp>
#include <sge/image2d/multi_loader.hpp>
#include <sge/image/color/format.hpp>
#include <sge/renderer/dim2.hpp>
#include <sge/renderer/glsl/scoped_program.hpp>
#include <sge/renderer/matrix4.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/renderer/state/state.hpp>
#include <sge/renderer/texture/texture.hpp>
#include <sge/shader/shader.hpp>
#include <sge/sprite/default_equal.hpp>
#include <sge/texture/add_image.hpp>
#include <sge/texture/fragmented_unique_ptr.hpp>
#include <sge/texture/part_raw.hpp>
#include <sge/texture/rect_fragmented.hpp>
#include <sge/texture/no_fragmented.hpp>
#include <fcppt/algorithm/map.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/container/ptr/push_back_unique_ptr.hpp>
#include <fcppt/random/make_last_exclusive_range.hpp>
#include <fcppt/filesystem/directory_iterator.hpp>
#include <fcppt/math/dim/dim.hpp>
#include <fcppt/move.hpp>
#include <fcppt/ref.hpp>
#include <fcppt/cref.hpp>
#include <fcppt/text.hpp>
#include <boost/next_prior.hpp>
#include <boost/spirit/home/phoenix/object.hpp>
#include <iostream>
#include <iterator>
#include <cstddef>

namespace
{
fruitcut::fruitlib::uniform_random<std::size_t>::type const
create_random_from_directory(
	fruitcut::fruitlib::random_generator const &_random_generator,
	fcppt::filesystem::path const &p)
{
	return 
		fruitcut::fruitlib::uniform_random<std::size_t>::type(
			fcppt::random::make_last_exclusive_range(
				static_cast<std::size_t>(
					0),
				static_cast<std::size_t>(
					std::distance(
						fcppt::filesystem::directory_iterator(
							p),
						fcppt::filesystem::directory_iterator()))),
				_random_generator);
}

sge::texture::part_ptr const
create_part_from_file(
	sge::image2d::multi_loader &image_loader,
	sge::texture::manager &texture_manager,
	fcppt::filesystem::path const &p)
{
	return 
		sge::texture::add_image(
			texture_manager,
			*image_loader.load(
				p));
}
}


fruitcut::app::point_sprite::system_node::system_node(
	fcppt::filesystem::path const &_base_path,
	fruitlib::random_generator const &_random_generator,
	sge::renderer::device &_renderer,
	sge::image2d::multi_loader &_image_loader,
	sge::camera::object const &_camera)
:
	renderer_(
		_renderer),
	camera_(
		_camera),
	texture_manager_(
		boost::phoenix::construct<sge::texture::fragmented_unique_ptr>(
			boost::phoenix::new_<sge::texture::no_fragmented>(
				fcppt::ref(
					_renderer),
				sge::image::color::format::rgba8,
				sge::renderer::texture::mipmap::off(),
				sge::renderer::texture::address_mode2(
					sge::renderer::texture::address_mode::clamp))
			/*
			boost::phoenix::new_<sge::texture::rect_fragmented>(
				fcppt::ref(
					_renderer),
				sge::image::color::format::rgba8,
				sge::renderer::texture::filter::linear,
				sge::renderer::dim2(
					512,
					512))*/)),
	system_(
		renderer_),
	children_(),
	textures_(
		fruitlib::resource_tree::from_directory_tree<resource_tree_type>(
			_base_path,
			std::tr1::bind(
				&create_part_from_file,
				fcppt::ref(
					_image_loader),
				fcppt::ref(
					texture_manager_),
				std::tr1::placeholders::_1),
			std::tr1::bind(
				&create_random_from_directory,
				fcppt::cref(
					_random_generator),
				std::tr1::placeholders::_1))),
	shader_(
		sge::shader::object_parameters(
			renderer_,
			system_.vertex_declaration(),
			fruitcut::media_path()/FCPPT_TEXT("shaders")/FCPPT_TEXT("point_sprite")/FCPPT_TEXT("vertex.glsl"),
			fruitcut::media_path()/FCPPT_TEXT("shaders")/FCPPT_TEXT("point_sprite")/FCPPT_TEXT("fragment.glsl"),
			sge::shader::vertex_format_string(
				""),
			fcppt::assign::make_container<sge::shader::variable_sequence>
				(sge::shader::variable(
					"mvp",
					sge::shader::variable_type::uniform,
					sge::shader::matrix(
						sge::renderer::matrix4(),
						sge::shader::matrix_flags::projection))),
			fcppt::assign::make_container<sge::shader::sampler_sequence>
				(sge::shader::sampler(
					"tex",
					sge::renderer::texture::planar_ptr()))))
{
}

void
fruitcut::app::point_sprite::system_node::push_back(
	unique_base_ptr n)
{
	fcppt::container::ptr::push_back_unique_ptr(
		children_,
		fcppt::move(
			n));
}

fruitcut::app::point_sprite::system &
fruitcut::app::point_sprite::system_node::system()
{
	return system_;
}

fruitcut::app::point_sprite::system const &
fruitcut::app::point_sprite::system_node::system() const
{
	return system_;
}

sge::texture::part_ptr const
fruitcut::app::point_sprite::system_node::lookup_texture(
	fruitlib::resource_tree::path const &target_path)
{
	resource_tree_type &target_tree =
		fruitlib::resource_tree::navigate_to_path(
			*textures_,
			target_path);

	if(target_tree.value().is_leaf())
		return 
			target_tree.value().leaf_value();

	resource_tree_type &target_file = 
		*boost::next(
			target_tree.begin(),
			static_cast<std::iterator_traits<resource_tree_type::const_iterator>::difference_type>(
				target_tree.value().node_value()()));

	if(!target_file.value().is_leaf())
		throw app::exception(FCPPT_TEXT("The argument to lookup_texture() must be either a file or a directory containing just files!\nThat was not the case for: ")+target_path.string());

	return 
		target_file.value().leaf_value();
}

fruitcut::app::point_sprite::system_node::~system_node()
{
}

void
fruitcut::app::point_sprite::system_node::update()
{
	for(child_sequence::iterator i = children_.begin(); i != children_.end();)
	{
		i->update();
		if(i->dead())
			i = 
				children_.erase(
					i);
		else
			i++;
	}
}

void
fruitcut::app::point_sprite::system_node::render()
{
	sge::shader::scoped scoped_shader(
		shader_,
		sge::shader::activate_bare());

	shader_.update_uniform(
		"mvp",
		sge::shader::matrix(
			camera_.mvp(),
			sge::shader::matrix_flags::projection));

	sge::renderer::state::scoped scoped_state(
		renderer_,
		sge::renderer::state::list
			(sge::renderer::state::depth_func::off));

	system_.render_all(
		sge::sprite::default_equal());
}
