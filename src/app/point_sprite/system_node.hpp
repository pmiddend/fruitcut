#ifndef FRUITCUT_APP_POINT_SPRITE_SYSTEM_NODE_HPP_INCLUDED
#define FRUITCUT_APP_POINT_SPRITE_SYSTEM_NODE_HPP_INCLUDED

#include "system.hpp"
#include "base.hpp"
#include "unique_base_ptr.hpp"
#include "../../fruitlib/scenic/nodes/intrusive.hpp"
#include "../../fruitlib/resource_tree/make_type.hpp"
#include "../../fruitlib/resource_tree/path.hpp"
#include "../../fruitlib/uniform_random.hpp"
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
#include <cstddef>

namespace fruitcut
{
namespace app
{
namespace point_sprite
{
class system_node
:
	public fruitlib::scenic::nodes::intrusive
{
FCPPT_NONCOPYABLE(
	system_node);
public:
	explicit
	system_node(
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

	void
	update();

	void
	render();
};
}
}
}

#endif
