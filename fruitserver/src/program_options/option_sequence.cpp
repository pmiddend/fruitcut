#include <fruitserver/program_options/option_sequence.hpp>


fruitserver::program_options::option_sequence::option_sequence()
:
	options_()
{
}

fruitserver::program_options::option_sequence &
fruitserver::program_options::option_sequence::operator()(
	detail::option_base const &new_option)
{
	options_.push_back(
		new_option);

	return
		*this;
}

fruitserver::program_options::option_sequence::const_iterator const
fruitserver::program_options::option_sequence::begin() const
{
	return options_.begin();
}

fruitserver::program_options::option_sequence::const_iterator const
fruitserver::program_options::option_sequence::end() const
{
	return options_.end();
}
