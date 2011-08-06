#ifndef FRUITAPP_BONSU_TEXTURE_MANAGER_HPP_INCLUDED
#define FRUITAPP_BONSU_TEXTURE_MANAGER_HPP_INCLUDED

#include <fruitapp/bonsu/texture_identifier.hpp>
#include <sge/texture/part_ptr.hpp>
#include <sge/texture/manager.hpp>
#include <sge/image2d/multi_loader_fwd.hpp>
#include <sge/renderer/device_fwd.hpp>
#include <fcppt/string.hpp>
#include <boost/unordered_map.hpp>

namespace fruitapp
{
namespace bonsu
{
class texture_manager
{
FCPPT_NONCOPYABLE(
	texture_manager);
public:
	explicit
	texture_manager(
		sge::renderer::device &,
		sge::image2d::multi_loader &);

	sge::texture::part_ptr const
	lookup(
		bonsu::texture_identifier const &);

	~texture_manager();
private:
	typedef
	boost::unordered_map
	<
		bonsu::texture_identifier,
		sge::texture::part_ptr
	>
	texture_map;

	sge::texture::manager texture_manager_;
	texture_map textures_;
};
}
}

#endif
