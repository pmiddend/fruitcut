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
	highscore::name::value_type name;
	app::score::value_type score_;
	boost::posix_time::ptime date_time_;

	explicit
	entry(
		highscore::name const &,
		fruitcut::app::score const &,
		boost::posix_time::ptime);

	bool 
	operator<(
		highscore::entry const &right) const;
};
}
}
}

#endif
