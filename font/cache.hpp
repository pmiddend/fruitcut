#ifndef FRUITCUT_FONT_CACHE_HPP_INCLUDED
#define FRUITCUT_FONT_CACHE_HPP_INCLUDED

#include "drawer/object_fwd.hpp"
#include "identifier.hpp"
#include <sge/font/system_ptr.hpp>
#include <sge/font/metrics_ptr.hpp>
#include <sge/renderer/device_ptr.hpp>
#include <sge/image2d/multi_loader_fwd.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <fcppt/string.hpp>
#include <fcppt/noncopyable.hpp>
#include <boost/ptr_container/ptr_list.hpp>
#include <list>
#include <map>

namespace fruitcut
{
namespace font
{
class cache
{
FCPPT_NONCOPYABLE(
	cache);
public:
	explicit
	cache(
		sge::font::system_ptr,
		sge::renderer::device_ptr,
		sge::image2d::multi_loader &,
		sge::parse::json::object const &);

	sge::font::metrics_ptr const
	metrics(
		identifier const &);

	fruitcut::font::drawer::object &
	drawer(
		identifier const &);

	~cache();
private:
	// We push elements and use iterators to the elements, so a list is
	// mandatory here!
	typedef
	std::list<sge::font::metrics_ptr>
	metrics_sequence;

	typedef
	boost::ptr_list<fruitcut::font::drawer::object>
	drawer_sequence;

	typedef
	std::map
	<
		fcppt::string,
		metrics_sequence::iterator
	>
	to_metrics;

	typedef
	std::map
	<
		fcppt::string,
		drawer_sequence::iterator
	>
	to_drawer;

	metrics_sequence metrics_;
	drawer_sequence drawers_;
	to_metrics to_metrics_;
	to_drawer to_drawer_;
};
}
}

#endif
