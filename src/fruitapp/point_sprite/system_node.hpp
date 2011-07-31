#ifndef FRUITAPP_POINT_SPRITE_SYSTEM_NODE_HPP_INCLUDED
#define FRUITAPP_POINT_SPRITE_SYSTEM_NODE_HPP_INCLUDED

#include <fruitapp/point_sprite/system.hpp>
#include <fruitapp/point_sprite/base.hpp>
#include <fruitapp/point_sprite/unique_base_ptr.hpp>
#include <fruitlib/scenic/node.hpp>
#include <fruitlib/scenic/optional_parent.hpp>
#include <fruitlib/scenic/events/update_fwd.hpp>
#include <fruitlib/scenic/events/render_fwd.hpp>
#include <fruitlib/resource_tree/make_type.hpp>
#include <fruitlib/resource_tree/path.hpp>
#include <fruitlib/uniform_random.hpp>
#include <sge/renderer/device_fwd.hpp>
#include <sge/camera/object_fwd.hpp>
#include <sge/texture/part_ptr.hpp>
#include <sge/texture/manager.hpp>
#include <sge/image2d/multi_loader.hpp>
#include <sge/shader/object.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/unique_ptr.hpp>
#include <fcppt/filesystem/path.hpp>
#include <boost/ptr_container/ptr_list.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <cstddef>

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
		fcppt::filesystem::path const &,
		fruitlib::random_generator &,
		sge::renderer::device &,
		sge::image2d::multi_loader &,
		sge::camera::object const &);

	void
	push_back(
		unique_base_ptr);

	point_sprite::system &
	system();

	point_sprite::system const &
	system() const;

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
		fruitlib::uniform_random
		<
			std::size_t
		>::type
	>::type
	resource_tree_type;

	typedef
	fcppt::unique_ptr<resource_tree_type>
	resource_tree_ptr;

	sge::renderer::device &renderer_;
	sge::camera::object const &camera_;
	sge::texture::manager texture_manager_;
	point_sprite::system system_;
	child_sequence children_;
	resource_tree_ptr textures_;
	sge::shader::object shader_;

};
}
}

#endif
