#ifndef FRUITCUT_APP_HIGHSCORE_ENTRY_HPP_INCLUDED
#define FRUITCUT_APP_HIGHSCORE_ENTRY_HPP_INCLUDED

#include "name.hpp"
#include "../score.hpp"
#include <boost/date_time/posix_time/posix_time.hpp>

namespace fruitcut
{
namespace app
{
namespace highscore
{
class entry
{
public:
	explicit
	entry(
		highscore::name const &,
		fruitcut::app::score const &,
		boost::posix_time::ptime);

	bool 
	operator<(
		highscore::entry const &right) const;

	highscore::name::value_type const &
	name() const;

	app::score::value_type
	score() const;

	boost::posix_time::ptime const
	date_time() const;
private:
	highscore::name::value_type name_;
	app::score::value_type score_;
	boost::posix_time::ptime date_time_;
};
}
}
}

#endif
