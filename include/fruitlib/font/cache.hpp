#ifndef FRUITLIB_FONT_CACHE_HPP_INCLUDED
#define FRUITLIB_FONT_CACHE_HPP_INCLUDED

#include <fruitlib/font/base_path.hpp>
#include <fruitlib/font/identifier.hpp>
#include <sge/font/object_fwd.hpp>
#include <sge/font/system_fwd.hpp>
#include <sge/image2d/system_fwd.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/string.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/ptr_container/ptr_list.hpp>
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
		sge::font::system &,
		sge::image2d::system &,
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
	boost::ptr_list<sge::font::object>
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