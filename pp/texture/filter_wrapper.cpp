#include "filter_wrapper.hpp"
#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_comparison.hpp>

fruitcut::pp::texture::filter_wrapper::filter_wrapper(
	sge::renderer::filter::texture const &_value)
:
	value_(
		_value)
{
}

sge::renderer::filter::texture const &
fruitcut::pp::texture::filter_wrapper::value() const
{
	return value_;
}

bool
fruitcut::pp::texture::filter_wrapper::operator<(
	filter_wrapper const &r) const
{
	return 
		boost::make_tuple(
			value().min(),
			value().mag(),
			value().anisotropy()) < 
		boost::make_tuple(
			r.value().min(),
			r.value().mag(),
			r.value().anisotropy());
}
