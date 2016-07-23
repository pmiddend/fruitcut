#ifndef FRUITLIB_FONT_CACHE_HPP_INCLUDED
#define FRUITLIB_FONT_CACHE_HPP_INCLUDED

#include <fruitlib/font/base_path.hpp>
#include <fruitlib/font/identifier.hpp>
#include <sge/font/object_fwd.hpp>
#include <sge/font/object_unique_ptr.hpp>
#include <sge/font/system_fwd.hpp>
#include <sge/image2d/system_fwd.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <sge/renderer/device/core_fwd.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/string.hpp>
#include <fcppt/log/context_fwd.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/filesystem/path.hpp>
#include <list>
#include <map>
#include <fcppt/config/external_end.hpp>


namespace fruitlib
{
namespace font
{
class cache
{
FCPPT_NONCOPYABLE(
	cache);
public:
	cache(
		fcppt::log::context &,
		sge::font::system &,
		sge::image2d::system &,
		sge::renderer::device::core const &,
		sge::parse::json::object const &,
		fruitlib::font::base_path const &);

	sge::font::object &
	get(
		fruitlib::font::identifier const &);

	~cache();
private:
	// We push elements and use iterators to the elements, so a list is
	// mandatory here!
	typedef
	std::list<sge::font::object_unique_ptr>
	object_sequence;

	typedef
	std::map
	<
		fruitlib::font::identifier,
		object_sequence::iterator
	>
	to_object;

	object_sequence objects_;
	to_object to_object_;
};
}
}

#endif
