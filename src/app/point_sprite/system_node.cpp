#include "system_node.hpp"
#include "../../media_path.hpp"
#include "../../exception.hpp"
#include <sge/camera/object.hpp>
#include <sge/image2d/file.hpp>
#include <sge/image2d/multi_loader.hpp>
#include <sge/renderer/glsl/scoped_program.hpp>
#include <sge/renderer/matrix4.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/renderer/texture/texture.hpp>
#include <sge/renderer/state/state.hpp>
#include <sge/shader/shader.hpp>
#include <sge/sprite/default_equal.hpp>
#include <sge/texture/part_raw.hpp>
#include <fcppt/algorithm/map.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/container/ptr/push_back_unique_ptr.hpp>
#include <fcppt/filesystem/directory_iterator.hpp>
#include <fcppt/filesystem/path_to_string.hpp>
#include <fcppt/filesystem/stem.hpp>
#include <fcppt/make_shared_ptr.hpp>
#include <fcppt/move.hpp>
#include <fcppt/ref.hpp>
#include <fcppt/text.hpp>
#include <iostream>

fruitcut::app::point_sprite::system_node::system_node(
	sge::renderer::device &_renderer,
	sge::image2d::multi_loader &_image_loader,
	sge::camera::object const &_camera)
:
	renderer_(
		_renderer),
	camera_(
		_camera),
	system_(
		renderer_),
	children_(),
	textures_(),
	shader_(
		renderer_,
		fruitcut::media_path()/FCPPT_TEXT("shaders")/FCPPT_TEXT("point_sprite_vertex.glsl"),
		fruitcut::media_path()/FCPPT_TEXT("shaders")/FCPPT_TEXT("point_sprite_fragment.glsl"),
		"",
		fcppt::assign::make_container<sge::shader::variable_sequence>
			(sge::shader::variable(
				"mvp",
				sge::shader::variable_type::uniform,
				sge::renderer::matrix4())),
		fcppt::assign::make_container<sge::shader::sampler_sequence>
			(sge::shader::sampler(
				"tex",
				sge::renderer::texture::planar_ptr())))
{
	for(
		fcppt::filesystem::directory_iterator current_filename = 
			fcppt::filesystem::directory_iterator(
				fruitcut::media_path()/FCPPT_TEXT("textures")/FCPPT_TEXT("splatter"));
		current_filename != fcppt::filesystem::directory_iterator();
		++current_filename)
	{
		textures_.insert(
			texture_map::value_type(
				fcppt::filesystem::path_to_string(
					fcppt::filesystem::stem(
						*current_filename)),
				fcppt::make_shared_ptr<sge::texture::part_raw>(
					sge::renderer::texture::create_planar_from_view(
						renderer_,
						_image_loader.load(
							*current_filename)->view(),
						sge::renderer::texture::filter::linear,
						sge::renderer::texture::address_mode2(
							sge::renderer::texture::address_mode::clamp),
						sge::renderer::resource_flags::none))));
	}
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
	fcppt::string const &s)
{
	texture_map::iterator i = 
		textures_.find(
			s);

	if(i == textures_.end())
		throw 
			fruitcut::exception(
				FCPPT_TEXT("Point sprite texture \"")+s+FCPPT_TEXT("\" was not found"));

	return i->second;
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
	sge::renderer::glsl::scoped_program scoped_shader(
		renderer_,
		shader_.program());

	shader_.update_uniform(
		"mvp",
		camera_.mvp());

	sge::renderer::state::scoped scoped_state(
		renderer_,
		sge::renderer::state::list(
			sge::renderer::state::depth_func::off));

	system_.render_all(
		sge::sprite::default_equal());
}
