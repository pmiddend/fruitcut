#ifndef FRUITAPP_POINT_SPRITE_SYSTEM_NODE_HPP_INCLUDED
#define FRUITAPP_POINT_SPRITE_SYSTEM_NODE_HPP_INCLUDED

#if 0
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
#include <sge/camera/base_fwd.hpp>
#include <sge/image2d/system_fwd.hpp>
#include <sge/renderer/device_fwd.hpp>
#include <sge/shader/object.hpp>
#include <sge/sprite/buffers/single_decl.hpp>
#include <sge/sprite/buffers/with_declaration_decl.hpp>
#include <sge/sprite/intrusive/collection_decl.hpp>
#include <sge/texture/manager.hpp>
#include <sge/texture/part_shared_ptr.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/shared_ptr.hpp>
#include <fcppt/unique_ptr.hpp>
#include <fcppt/preprocessor/pure.hpp>
#include <fcppt/preprocessor/warn_unused_result.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/ptr_container/ptr_list.hpp>
#include <cstddef>
#include <fcppt/config/external_end.hpp>
#endif


namespace fruitapp
{
namespace point_sprite
{
#if 0
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

	system_node(
		fruitlib::scenic::optional_parent const &,
		boost::filesystem::path const &,
		fruitlib::random_generator &,
		sge::renderer::device::core &,
		sge::image2d::system &,
		sge::camera::base const &);

	void
	push_back(
		fruitapp::point_sprite::unique_base_ptr);

	fruitapp::point_sprite::connection &
	connection()
	FCPPT_PP_WARN_UNUSED_RESULT;

	sge::texture::part_shared_ptr const
	lookup_texture(
		fruitlib::resource_tree::path const &)
	FCPPT_PP_WARN_UNUSED_RESULT
	FCPPT_PP_PURE;

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
		sge::texture::part_shared_ptr,
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
	sge::camera::base const &camera_;
	sge::texture::manager texture_manager_;
	point_sprite::buffers buffers_;
	point_sprite::collection collection_;
	child_sequence children_;
	resource_tree_ptr textures_;
	sge::shader::object shader_;

};
#else
class system_node {};
#endif
}
}

#endif
