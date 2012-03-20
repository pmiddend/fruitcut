#ifndef FRUITAPP_HIGHSCORE_ENTRY_HPP_INCLUDED
#define FRUITAPP_HIGHSCORE_ENTRY_HPP_INCLUDED

#include <fruitapp/highscore/name.hpp>
#include <fruitapp/highscore/score.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
namespace highscore
{
class entry
{
public:
	explicit
	entry(
		highscore::name const &,
		highscore::score const &,
		boost::posix_time::ptime);

	bool
	operator<(
		highscore::entry const &right) const;

	highscore::name::value_type const &
	name() const;

	highscore::score::value_type
	score() const;

	boost::posix_time::ptime const
	date_time() const;
private:
	highscore::name::value_type name_;
	highscore::score::value_type score_;
	boost::posix_time::ptime date_time_;
};
}
}

#endif
