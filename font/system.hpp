#ifndef FRUITCUT_FONT_SYSTEM_HPP_INCLUDED
#define FRUITCUT_FONT_SYSTEM_HPP_INCLUDED

#include "particle/base.hpp"
#include "drawer.hpp"
#include "particle/unique_base_ptr.hpp"
#include <sge/renderer/device_ptr.hpp>
#include <sge/font/size_type.hpp>
#include <sge/font/metrics_ptr.hpp>
#include <sge/font/system_ptr.hpp>
#include <sge/image2d/multi_loader_fwd.hpp>
#include <fcppt/filesystem/path.hpp>
#include <boost/ptr_container/ptr_list.hpp>
#include <boost/intrusive/list.hpp>
#include <boost/ptr_container/ptr_map.hpp>
#include <boost/tuple/tuple_comparison.hpp>
#include <boost/tuple/tuple.hpp>

namespace fruitcut
{
namespace font
{
class system
{
public:
	explicit
	system(
		sge::renderer::device_ptr,
		sge::font::system_ptr,
		sge::image2d::multi_loader &);

	void
	insert(
		particle::unique_base_ptr);

	void
	insert(
		particle::base &);

	void
	update();

	void
	render();

	sge::font::metrics_ptr const
	load_ttf(
		fcppt::filesystem::path const &,
		sge::font::size_type);

	sge::font::metrics_ptr const
	load_bitmap(
		fcppt::filesystem::path const &);

	~system();
private:
	typedef
	boost::ptr_list<particle::base>
	nonintrusive_list;

	typedef
	boost::intrusive::list
	<
		particle::base,
		boost::intrusive::constant_time_size<false>
	>
	intrusive_list;

	typedef
	std::map
	<
		fcppt::filesystem::path,
		sge::font::metrics_ptr
	>
	bitmap_to_metrics;

	typedef
	std::map
	<
		boost::tuple
		<
			fcppt::filesystem::path,
			sge::font::size_type
		>,
		sge::font::metrics_ptr
	>
	ttf_to_metrics;

	typedef
	boost::ptr_map
	<
		sge::font::metrics_ptr,
		drawer
	>
	metrics_to_drawer;

	sge::renderer::device_ptr renderer_;
	sge::font::system_ptr font_system_;
	sge::image2d::multi_loader &image_loader_;
	intrusive_list intrusive_particles_;
	nonintrusive_list nonintrusive_particles_;
	bitmap_to_metrics bitmap_to_metrics_;
	ttf_to_metrics ttf_to_metrics_;
	metrics_to_drawer metrics_to_drawer_;

	void
	create_drawer(
		sge::font::metrics_ptr);

	void
	render(
		particle::base &);
};
}
}

#endif
