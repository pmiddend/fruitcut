#ifndef FRUITAPP_POINT_SPRITE_SYSTEM_NODE_HPP_INCLUDED
#define FRUITAPP_POINT_SPRITE_SYSTEM_NODE_HPP_INCLUDED

#include <fruitapp/point_sprite/base.hpp>
#include <fruitapp/point_sprite/buffers.hpp>
#include <fruitapp/point_sprite/collection.hpp>
#include <fruitapp/point_sprite/connection.hpp>
#include <fruitapp/point_sprite/unique_base_ptr.hpp>
#include <fruitlib/uniform_int_random.hpp>
#include <fruitlib/resource_tree/make_type.hpp>
#include <fruitlib/resource_tree/path.hpp>
#include <fruitlib/scenic/node.hpp>
#include <fruitlib/scenic/optional_parent.hpp>
#include <fruitlib/scenic/events/render.hpp>
#include <fruitlib/scenic/events/update.hpp>
#include <sge/camera/first_person/object_fwd.hpp>
#include <sge/image2d/system_fwd.hpp>
#include <sge/renderer/device_fwd.hpp>
#include <sge/shader/object.hpp>
#include <sge/sprite/buffers/single_decl.hpp>
#include <sge/sprite/buffers/with_declaration_decl.hpp>
#include <sge/sprite/intrusive/collection_decl.hpp>
#include <sge/texture/manager.hpp>
#include <sge/texture/part_ptr.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/unique_ptr.hpp>
#include <fcppt/shared_ptr.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/ptr_container/ptr_list.hpp>
#include <cstddef>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
namespace point_sprite
{
class system_node
:
	public fruitlib::scenic::node<system_node>
{
FCPPT_NONCOPYABLE(
	system_node);
public:
	typedef
	boost::mpl::vector2<fruitlib::scenic::events::render,fruitlib::scenic::events::update>
	scene_reactions;

	explicit
	system_node(
		fruitlib::scenic::optional_parent const &,
		boost::filesystem::path const &,
		fruitlib::random_generator &,
		sge::renderer::device &,
		sge::image2d::system &,
		sge::camera::first_person::object const &);

	void
	push_back(
		unique_base_ptr);

	point_sprite::connection &
	connection();

	sge::texture::part_ptr const
	lookup_texture(
		fruitlib::resource_tree::path const &);

	~system_node();

	void
	react(
		fruitlib::scenic::events::update const &);

	void
	react(
		fruitlib::scenic::events::render const &);
private:
	typedef
	boost::ptr_list<point_sprite::base>
	child_sequence;

	typedef
	fruitlib::resource_tree::make_type
	<
		sge::texture::part_ptr,
		// shared_ptr because of horrible unique_ptr semantics
		fcppt::shared_ptr
		<
			fruitlib::uniform_int_random<std::size_t>::type
		>
	>::type
	resource_tree_type;

	typedef
	fcppt::unique_ptr<resource_tree_type>
	resource_tree_ptr;

	sge::renderer::device &renderer_;
	sge::camera::first_person::object const &camera_;
	sge::texture::manager texture_manager_;
	point_sprite::buffers buffers_;
	point_sprite::collection collection_;
	child_sequence children_;
	resource_tree_ptr textures_;
	sge::shader::object shader_;

};
}
}

#endif
