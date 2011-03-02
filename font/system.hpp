#ifndef FRUITCUT_FONT_SYSTEM_HPP_INCLUDED
#define FRUITCUT_FONT_SYSTEM_HPP_INCLUDED

#include "particle/base.hpp"
#include "drawer.hpp"
#include "cache_fwd.hpp"
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
		cache &);

	void
	insert(
		particle::base &);

	void
	update();

	void
	render();

	~system();
private:
	typedef
	boost::intrusive::list
	<
		particle::base,
		boost::intrusive::constant_time_size<false>
	>
	intrusive_list;

	cache &cache_;
	intrusive_list intrusive_particles_;
};
}
}

#endif
