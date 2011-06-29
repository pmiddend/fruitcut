#include "option_sequence.hpp"

fruitcut::server::program_options::option_sequence::option_sequence()
:
	options_()
{
}

fruitcut::server::program_options::option_sequence &
fruitcut::server::program_options::option_sequence::operator()(
	detail::option_base const &new_option)
{
	options_.push_back(
		new_option);

	return 
		*this;
}

fruitcut::server::program_options::option_sequence::const_iterator const
fruitcut::server::program_options::option_sequence::begin() const
{
	return options_.begin();
}

fruitcut::server::program_options::option_sequence::const_iterator const
fruitcut::server::program_options::option_sequence::end() const
{
	return options_.end();
}
