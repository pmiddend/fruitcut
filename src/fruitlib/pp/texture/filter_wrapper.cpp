#include "filter_wrapper.hpp"
#include <boost/fusion/tuple.hpp>

fruitcut::fruitlib::pp::texture::filter_wrapper::filter_wrapper(
	sge::renderer::texture::filter::object const &_value)
:
	value_(
		_value)
{
}

sge::renderer::texture::filter::object const &
fruitcut::fruitlib::pp::texture::filter_wrapper::value() const
{
	return value_;
}

bool
fruitcut::fruitlib::pp::texture::filter_wrapper::operator<(
	filter_wrapper const &r) const
{
	return 
		boost::fusion::make_tuple(
			value().min(),
			value().mag(),
			value().anisotropy()) < 
		boost::fusion::make_tuple(
			r.value().min(),
			r.value().mag(),
			r.value().anisotropy());
}
