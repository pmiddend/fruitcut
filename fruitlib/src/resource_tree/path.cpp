#include <fruitlib/resource_tree/path.hpp>
#include <fcppt/string.hpp>
#include <fcppt/text.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/next_prior.hpp>
#include <fcppt/config/external_end.hpp>


fruitlib::resource_tree::path::path(
	fcppt::string const &s)
:
	values_(
		static_cast<value_sequence::size_type>(1))
{
	values_[0] = s;
}

fruitlib::resource_tree::path const
fruitlib::resource_tree::path::operator/(
	fcppt::string const &s) const
{
	resource_tree::path copy(
		*this);
	copy.values_.push_back(
		s);
	return
		copy;
}

fruitlib::resource_tree::path::value_sequence const &
fruitlib::resource_tree::path::values() const
{
	return values_;
}

fcppt::string const
fruitlib::resource_tree::path::string() const
{
	fcppt::string result;
	for(
		value_sequence::const_iterator i = values_.begin();
		i != values_.end();
		++i)
	{
		result += *i;
		if(i != boost::prior(values_.end()))
			result += FCPPT_TEXT("/");
	}
	return result;
}
