#ifndef FRUITAPP_POINT_SPRITE_SYSTEM_NODE_HPP_INCLUDED
#define FRUITAPP_POINT_SPRITE_SYSTEM_NODE_HPP_INCLUDED

#include <fruitapp/point_sprite/base.hpp>
#include <fruitapp/point_sprite/buffers.hpp>
#include <fruitapp/point_sprite/collection.hpp>
#include <fruitapp/point_sprite/connection.hpp>
#include <fruitapp/point_sprite/state_object.hpp>
#include <fruitapp/point_sprite/unique_base_ptr.hpp>
#include <fruitapp/projection_manager/object_fwd.hpp>
#include <fruitlib/texture_manager_fwd.hpp>
#include <fruitlib/uniform_int_random.hpp>
#include <fruitlib/resource_tree/make_type.hpp>
#include <fruitlib/resource_tree/path.hpp>
#include <fruitlib/scenic/node.hpp>
#include <fruitlib/scenic/optional_parent.hpp>
#include <fruitlib/scenic/events/render.hpp>
#include <fruitlib/scenic/events/update.hpp>
#include <sge/renderer/device/ffp_fwd.hpp>
#include <sge/sprite/buffers/single_decl.hpp>
#include <sge/sprite/buffers/with_declaration_decl.hpp>
#include <sge/sprite/intrusive/collection_decl.hpp>
#include <sge/texture/const_optional_part_ref.hpp>
#include <sge/texture/manager.hpp>
#include <sge/texture/part_shared_ptr.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/shared_ptr.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <cstddef>
#include <list>
#include <memory>
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
	boost::mpl::vector2
	<
		fruitlib::scenic::events::render,
		fruitlib::scenic::events::update
	>
	scene_reactions;

	system_node(
		fruitlib::scenic::optional_parent const &,
		boost::filesystem::path const &,
		fruitlib::random_generator &,
		sge::renderer::device::ffp &,
		fruitapp::projection_manager::object const &,
		fruitlib::texture_manager &);

	void
	push_back(
		fruitapp::point_sprite::unique_base_ptr);

	fruitapp::point_sprite::connection &
	connection();

	sge::texture::const_optional_part_ref const
	lookup_texture(
		fruitlib::resource_tree::path const &);

	~system_node();

	void
	react(
		fruitlib::scenic::events::update const &);

	void
	react(
		fruitlib::scenic::events::render const &);

	fruitapp::projection_manager::object const &
	projection_manager() const;
private:
	typedef
	std::list
	<
		std::unique_ptr
		<
			fruitapp::point_sprite::base
		>
	>
	child_sequence;

	typedef
	fruitlib::resource_tree::make_type
	<
		sge::texture::part_shared_ptr,
		// shared_ptr because of horrible unique_ptr semantics
		fcppt::shared_ptr
		<
			fruitlib::uniform_int_random<std::size_t>::type
		>
	>::type
	resource_tree_type;

	sge::renderer::device::ffp &renderer_;
	fruitapp::projection_manager::object const &projection_manager_;
	sge::texture::manager texture_manager_;
	fruitapp::point_sprite::buffers buffers_;
	fruitapp::point_sprite::collection collection_;
	fruitapp::point_sprite::state_object states_;
	child_sequence children_;
	resource_tree_type textures_;

};
}
}

#endif
