#include <fruitapp/exception.hpp>
#include <fruitapp/point_sprite/system_node.hpp>
#include <fruitlib/media_path.hpp>
#include <fruitlib/resource_tree/from_directory_tree.hpp>
#include <fruitlib/resource_tree/navigate_to_path.hpp>
#include <fruitlib/resource_tree/path.hpp>
#include <sge/camera/first_person/object.hpp>
#include <sge/image/color/format.hpp>
#include <sge/image2d/file.hpp>
#include <sge/image2d/system.hpp>
#include <sge/renderer/device_fwd.hpp>
#include <sge/renderer/dim2.hpp>
#include <sge/renderer/matrix4.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/renderer/glsl/scoped_program.hpp>
#include <sge/renderer/state/depth_func.hpp>
#include <sge/renderer/state/list.hpp>
#include <sge/renderer/state/scoped.hpp>
#include <sge/renderer/state/trampoline.hpp>
#include <sge/renderer/texture/planar_ptr.hpp>
#include <sge/renderer/texture/mipmap/off.hpp>
#include <sge/shader/activate_bare.hpp>
#include <sge/shader/matrix.hpp>
#include <sge/shader/matrix_flags.hpp>
#include <sge/shader/object_parameters.hpp>
#include <sge/shader/sampler.hpp>
#include <sge/shader/sampler_sequence.hpp>
#include <sge/shader/scoped.hpp>
#include <sge/shader/variable.hpp>
#include <sge/shader/variable_sequence.hpp>
#include <sge/shader/variable_type.hpp>
#include <sge/shader/vertex_format_string.hpp>
#include <sge/sprite/buffers/option.hpp>
#include <sge/sprite/buffers/single_impl.hpp>
#include <sge/sprite/buffers/with_declaration_impl.hpp>
#include <sge/sprite/compare/default.hpp>
#include <sge/sprite/intrusive/collection_impl.hpp>
#include <sge/sprite/process/all.hpp>
#include <sge/texture/add_image.hpp>
#include <sge/texture/fragmented_unique_ptr.hpp>
#include <sge/texture/manager_fwd.hpp>
#include <sge/texture/no_fragmented.hpp>
#include <sge/texture/part_ptr.hpp>
#include <sge/texture/part_raw.hpp>
#include <sge/texture/rect_fragmented.hpp>
#include <fcppt/move.hpp>
#include <fcppt/ref.hpp>
#include <fcppt/text.hpp>
#include <fcppt/algorithm/map.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/container/ptr/push_back_unique_ptr.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/next_prior.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/spirit/home/phoenix/object.hpp>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <fcppt/config/external_end.hpp>


namespace
{
fcppt::shared_ptr
<
	fruitlib::uniform_int_random<std::size_t>::type
>
create_random_from_directory(
	fruitlib::random_generator &_random_generator,
	boost::filesystem::path const &p)
{
	typedef
	fruitlib::uniform_int_random<std::size_t>::type
	random_variate;

	return
		fcppt::make_shared_ptr<random_variate>(
			fcppt::ref(
				_random_generator),
			random_variate::distribution(
				random_variate::distribution::min(
					static_cast<std::size_t>(
						0)),
				random_variate::distribution::max(
					static_cast<std::size_t>(
						std::distance(
							boost::filesystem::directory_iterator(
								p),
							boost::filesystem::directory_iterator())-1))));
}

sge::texture::part_ptr const
create_part_from_file(
	sge::image2d::system &image_loader,
	sge::texture::manager &texture_manager,
	boost::filesystem::path const &p)
{
	return
		sge::texture::add_image(
			texture_manager,
			*image_loader.load(
				p));
}
}


fruitapp::point_sprite::system_node::system_node(
	fruitlib::scenic::optional_parent const &_parent,
	boost::filesystem::path const &_base_path,
	fruitlib::random_generator &_random_generator,
	sge::renderer::device &_renderer,
	sge::image2d::system &_image_loader,
	sge::camera::first_person::object const &_camera)
:
	node_base(
		_parent),
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
				sge::renderer::texture::mipmap::off()))),
	buffers_(
		renderer_,
		sge::sprite::buffers::option::dynamic),
	collection_(),
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
				fcppt::ref(
					_random_generator),
				std::tr1::placeholders::_1))),
	shader_(
		sge::shader::object_parameters(
			renderer_,
			buffers_.parameters().vertex_declaration(),
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
					sge::renderer::texture::planar_ptr())))
				.name(
					FCPPT_TEXT("point sprite"))
				.vertex_shader(
					fruitlib::media_path()/FCPPT_TEXT("shaders")/FCPPT_TEXT("point_sprite")/FCPPT_TEXT("vertex.glsl"))
				.fragment_shader(
					fruitlib::media_path()/FCPPT_TEXT("shaders")/FCPPT_TEXT("point_sprite")/FCPPT_TEXT("fragment.glsl")))
{
}

void
fruitapp::point_sprite::system_node::push_back(
	unique_base_ptr n)
{
	fcppt::container::ptr::push_back_unique_ptr(
		children_,
		fcppt::move(
			n));
}

fruitapp::point_sprite::connection &
fruitapp::point_sprite::system_node::connection()
{
	return collection_.connection();
}

sge::texture::part_ptr const
fruitapp::point_sprite::system_node::lookup_texture(
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
				(*target_tree.value().node_value())()));

	if(!target_file.value().is_leaf())
		throw fruitapp::exception(FCPPT_TEXT("The argument to lookup_texture() must be either a file or a directory containing just files!\nThat was not the case for: ")+target_path.string());

	return
		target_file.value().leaf_value();
}

fruitapp::point_sprite::system_node::~system_node()
{
}

void
fruitapp::point_sprite::system_node::react(
	fruitlib::scenic::events::update const &)
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
fruitapp::point_sprite::system_node::react(
	fruitlib::scenic::events::render const &)
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

	sge::sprite::process::all(
		collection_.range(),
		buffers_,
		sge::sprite::compare::default_());
}
