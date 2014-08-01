#include <fruitapp/exception.hpp>
#include <fruitapp/point_sprite/state_parameters.hpp>
#include <fruitapp/point_sprite/system_node.hpp>
#include <fruitlib/texture_manager.hpp>
#include <fruitlib/resource_tree/from_directory_tree.hpp>
#include <fruitlib/resource_tree/navigate_to_path.hpp>
#include <fruitlib/resource_tree/path.hpp>
#include <sge/image/color/format.hpp>
#include <sge/image2d/file.hpp>
#include <sge/image2d/system.hpp>
#include <sge/image2d/view/const_object.hpp>
#include <sge/renderer/dim2.hpp>
#include <sge/renderer/matrix4.hpp>
#include <sge/renderer/context/ffp.hpp>
#include <sge/renderer/device/ffp.hpp>
#include <sge/renderer/device/ffp_fwd.hpp>
#include <sge/renderer/texture/color_format.hpp>
#include <sge/renderer/texture/planar_shared_ptr.hpp>
#include <sge/renderer/texture/mipmap/off.hpp>
#include <sge/sprite/buffers/option.hpp>
#include <sge/sprite/buffers/single_impl.hpp>
#include <sge/sprite/buffers/with_declaration_impl.hpp>
#include <sge/sprite/compare/default.hpp>
#include <sge/sprite/intrusive/collection_impl.hpp>
#include <sge/sprite/process/all.hpp>
#include <sge/texture/fragmented.hpp>
#include <sge/texture/fragmented_unique_ptr.hpp>
#include <sge/texture/manager_fwd.hpp>
#include <sge/texture/part_shared_ptr.hpp>
#include <sge/texture/rect_fragmented.hpp>
#include <fcppt/make_shared_ptr.hpp>
#include <fcppt/text.hpp>
#include <fcppt/algorithm/map.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/next_prior.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <cstddef>
#include <functional>
#include <iterator>
#include <utility>
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
			_random_generator,
			random_variate::distribution(
				random_variate::distribution::param_type::min(
					static_cast<std::size_t>(
						0)),
				random_variate::distribution::param_type::max(
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
	fruitapp::projection_manager::object const &_projection_manager,
	fruitlib::texture_manager &_texture_manager)
:
	node_base(
		_parent),
	renderer_(
		_renderer),
	projection_manager_(
		_projection_manager),
	texture_manager_(
		std::bind(
			&fruitlib::texture_manager::create_rect_fragmented,
			&_texture_manager,
			sge::renderer::texture::mipmap::off(),
			std::placeholders::_1,
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
			std::bind(
				&create_part_from_file,
				std::ref(
					_texture_manager),
				std::ref(
					texture_manager_),
				std::placeholders::_1),
			std::bind(
				&create_random_from_directory,
				std::ref(
					_random_generator),
				std::placeholders::_1)))
{
}

void
fruitapp::point_sprite::system_node::push_back(
	unique_base_ptr n)
{
	children_.push_back(
		std::move(
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
			textures_,
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
		(*i)->update();
		if((*i)->dead())
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

fruitapp::projection_manager::object const &
fruitapp::point_sprite::system_node::projection_manager() const
{
	return
		projection_manager_;
}
