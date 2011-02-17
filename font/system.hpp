#ifndef FRUITCUT_FONT_SYSTEM_HPP_INCLUDED
#define FRUITCUT_FONT_SYSTEM_HPP_INCLUDED

#include "particle/base.hpp"
#include "drawer.hpp"
#include "particle/unique_base_ptr.hpp"
#include <sge/renderer/device_ptr.hpp>
#include <boost/ptr_container/ptr_list.hpp>
#include <boost/intrusive/list.hpp>

namespace fruitcut
{
namespace font
{
class system
{
public:
	explicit
	system(
		sge::renderer::device_ptr);

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

	drawer drawer_;
	intrusive_list intrusive_particles_;
	nonintrusive_list nonintrusive_particles_;
};
}
}

#endif
