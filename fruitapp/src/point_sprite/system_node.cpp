#include <fruitapp/exception.hpp>
#include <fruitlib/texture_manager.hpp>
#include <sge/texture/fragmented.hpp>
#include <fcppt/tr1/functional.hpp>
#include <fruitapp/point_sprite/state_parameters.hpp>
#include <fruitapp/point_sprite/system_node.hpp>
#include <fruitlib/resource_tree/from_directory_tree.hpp>
#include <fruitlib/resource_tree/navigate_to_path.hpp>
#include <fruitlib/resource_tree/path.hpp>
#include <sge/camera/base.hpp>
#include <sge/camera/coordinate_system/object.hpp>
#include <sge/camera/matrix_conversion/world_projection.hpp>
#include <sge/image/color/format.hpp>
#include <sge/image2d/file.hpp>
#include <sge/image2d/system.hpp>
#include <sge/image2d/view/const_object.hpp>
#include <sge/renderer/dim2.hpp>
#include <sge/renderer/matrix4.hpp>
#include <sge/renderer/context/ffp.hpp>
#include <sge/renderer/device/ffp.hpp>
#include <sge/renderer/device/ffp_fwd.hpp>
#include <sge/renderer/target/onscreen.hpp>
#include <sge/renderer/texture/color_format.hpp>
#include <sge/renderer/texture/planar_shared_ptr.hpp>
#include <sge/renderer/texture/mipmap/off.hpp>
#include <sge/sprite/buffers/option.hpp>
#include <sge/sprite/buffers/single_impl.hpp>
#include <sge/sprite/buffers/with_declaration_impl.hpp>
#include <sge/sprite/compare/default.hpp>
#include <sge/sprite/intrusive/collection_impl.hpp>
#include <sge/sprite/process/all.hpp>
#include <sge/texture/fragmented_unique_ptr.hpp>
#include <sge/texture/manager_fwd.hpp>
#include <sge/texture/part_shared_ptr.hpp>
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

sge::texture::part_shared_ptr const
create_part_from_file(
	fruitlib::texture_manager &_fruitlib_texture_manager,
	sge::texture::manager &texture_manager,
	boost::filesystem::path const &p)
{
	return
		sge::texture::part_shared_ptr(
			texture_manager.add(
				_fruitlib_texture_manager.create_image_from_path(
					p)->view()));
}
}


fruitapp::point_sprite::system_node::system_node(
	fruitlib::scenic::optional_parent const &_parent,
	boost::filesystem::path const &_base_path,
	fruitlib::random_generator &_random_generator,
	sge::renderer::device::ffp &_renderer,
	fruitlib::texture_manager &_texture_manager,
	sge::camera::base const &_camera)
:
	node_base(
		_parent),
	renderer_(
		_renderer),
	camera_(
		_camera),
	texture_manager_(
		std::tr1::bind(
			&fruitlib::texture_manager::create_rect_fragmented,
			&_texture_manager,
			sge::renderer::texture::mipmap::off(),
			std::tr1::placeholders::_1,
			sge::renderer::dim2(
				1024,
				1024))),
	buffers_(
		renderer_,
		sge::sprite::buffers::option::dynamic),
	collection_(),
	states_(
		renderer_,
		fruitapp::point_sprite::state_parameters()),
	children_(),
	textures_(
		fruitlib::resource_tree::from_directory_tree<resource_tree_type>(
			_base_path,
			std::tr1::bind(
				&create_part_from_file,
				fcppt::ref(
					_texture_manager),
				fcppt::ref(
					texture_manager_),
				std::tr1::placeholders::_1),
			std::tr1::bind(
				&create_random_from_directory,
				fcppt::ref(
					_random_generator),
				std::tr1::placeholders::_1)))
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

sge::texture::const_optional_part_ref const
fruitapp::point_sprite::system_node::lookup_texture(
	fruitlib::resource_tree::path const &target_path)
{
	resource_tree_type &target_tree =
		fruitlib::resource_tree::navigate_to_path(
			*textures_,
			target_path);

	if(target_tree.value().is_leaf())
		return
			sge::texture::const_optional_part_ref(
				*target_tree.value().leaf_value());

	resource_tree_type &target_file =
		*boost::next(
			target_tree.begin(),
			static_cast<std::iterator_traits<resource_tree_type::const_iterator>::difference_type>(
				(*target_tree.value().node_value())()));

	if(!target_file.value().is_leaf())
		throw
			fruitapp::exception(
				FCPPT_TEXT("The argument to lookup_texture() must be either a file or a directory containing just files!\nThat was not the case for: ")+
				target_path.string());

	return
		sge::texture::const_optional_part_ref(
			*target_file.value().leaf_value());
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
	fruitlib::scenic::events::render const &_render_event)
{
	sge::sprite::process::all(
		_render_event.context(),
		collection_.range(),
		buffers_,
		sge::sprite::compare::default_(),
		states_);
}

sge::camera::base const &
fruitapp::point_sprite::system_node::camera() const
{
	return
		camera_;
}

sge::renderer::target::base const &
fruitapp::point_sprite::system_node::target() const
{
	return
		renderer_.onscreen_target();
}
