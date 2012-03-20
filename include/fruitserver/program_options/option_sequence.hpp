#ifndef FRUITSERVER_PROGRAM_OPTIONS_OPTION_SEQUENCE_HPP_INCLUDED
#define FRUITSERVER_PROGRAM_OPTIONS_OPTION_SEQUENCE_HPP_INCLUDED

#include <fruitserver/program_options/detail/option_base.hpp>
#include <vector>


namespace fruitserver
{
namespace program_options
{
class option_sequence
{
private:
	typedef
	std::vector<detail::option_base>
	sequence;
public:
	typedef
	sequence::const_iterator
	const_iterator;

	explicit
	option_sequence();

	option_sequence &
	operator()(
		detail::option_base const &);

	const_iterator const
	begin() const;

	const_iterator const
	end() const;
private:
	sequence options_;
};
}
}

#endif
