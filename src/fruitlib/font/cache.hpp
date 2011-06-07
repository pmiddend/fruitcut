#ifndef FRUITCUT_FRUITLIB_FONT_CACHE_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_FONT_CACHE_HPP_INCLUDED

#include "drawer/object_fwd.hpp"
#include "identifier.hpp"
#include <sge/font/system_fwd.hpp>
#include <sge/font/metrics_ptr.hpp>
#include <sge/renderer/device_fwd.hpp>
#include <sge/image2d/multi_loader_fwd.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <fcppt/string.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/filesystem/path.hpp>
#include <boost/ptr_container/ptr_list.hpp>
#include <list>
#include <map>

namespace fruitcut
{
namespace fruitlib
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
		sge::font::system &,
		sge::renderer::device &,
		sge::image2d::multi_loader &,
		fcppt::filesystem::path const &base_path,
		sge::parse::json::object const &);

	sge::font::metrics_ptr const
	metrics(
		identifier const &);

	fruitcut::fruitlib::font::drawer::object &
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
	boost::ptr_list<fruitcut::fruitlib::font::drawer::object>
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
	fcppt::filesystem::path const base_path_;
};
}
}
}

#endif