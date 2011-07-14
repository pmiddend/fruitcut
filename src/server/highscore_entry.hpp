#ifndef FRUITCUT_SERVER_HIGHSCORE_ENTRY_HPP_INCLUDED
#define FRUITCUT_SERVER_HIGHSCORE_ENTRY_HPP_INCLUDED

#include <string>

namespace fruitcut
{
namespace server
{
class highscore_entry
{
public:
	explicit
	highscore_entry();

	explicit
	highscore_entry(
		std::string const &name,
		std::string const &score,
		std::string const &datetime);

	std::string const &
	name() const;

	std::string const & 
	score() const;

	unsigned 
	score_numerical() const;

	std::string const &
	datetime() const;

	bool
	operator==(
		highscore_entry const &) const;
private:
	std::string name_;
	std::string score_;
	unsigned score_numerical_;
	std::string datetime_;
};
}
}

#endif
