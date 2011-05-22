#ifndef FRUITCUT_APP_POINT_SPRITE_SYSTEM_NODE_HPP_INCLUDED
#define FRUITCUT_APP_POINT_SPRITE_SYSTEM_NODE_HPP_INCLUDED

#include "system.hpp"
#include "base.hpp"
#include "unique_base_ptr.hpp"
#include "../../scenic/nodes/intrusive.hpp"
#include <sge/renderer/device_fwd.hpp>
#include <sge/camera/object_fwd.hpp>
#include <sge/texture/part_ptr.hpp>
#include <sge/image2d/multi_loader.hpp>
#include <sge/shader/object.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/tr1/unordered_map.hpp>
#include <boost/ptr_container/ptr_list.hpp>

namespace fruitcut
{
namespace app
{
namespace point_sprite
{
class system_node
:
	public scenic::nodes::intrusive
{
FCPPT_NONCOPYABLE(
	system_node);
public:
	explicit
	system_node(
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
		fcppt::string const &);

	~system_node();
private:
	typedef
	boost::ptr_list<point_sprite::base>
	child_sequence;

	typedef
	std::tr1::unordered_map<fcppt::string,sge::texture::part_ptr>
	texture_map;

	sge::renderer::device &renderer_;
	sge::camera::object const &camera_;
	point_sprite::system system_;
	child_sequence children_;
	texture_map textures_;
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
