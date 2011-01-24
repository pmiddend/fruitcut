#ifndef FRUITCUT_PP_TEXTURE_MANAGER_HPP_INCLUDED
#define FRUITCUT_PP_TEXTURE_MANAGER_HPP_INCLUDED

#include "descriptor.hpp"
#include "instance.hpp"
#include "counted_instance.hpp"
#include <sge/renderer/device_ptr.hpp>
#include <sge/renderer/texture_ptr.hpp>
#include <fcppt/noncopyable.hpp>
#include <boost/ptr_container/ptr_map.hpp>

namespace fruitcut
{
namespace pp
{
namespace texture
{
class manager
{
FCPPT_NONCOPYABLE(manager)
public:
	explicit
	manager(
		sge::renderer::device_ptr);

	counted_instance const
	query(
		descriptor const &);

	~manager();
private:
	typedef
	boost::ptr_multimap
	<
		descriptor,
		instance
	>
	texture_map;

	sge::renderer::device_ptr renderer_;
	texture_map textures_;
};
}
}
}

#endif
