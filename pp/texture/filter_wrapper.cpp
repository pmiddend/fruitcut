#include "filter_wrapper.hpp"
#include <fcppt/tr1/tuple.hpp>

fruitcut::pp::texture::filter_wrapper::filter_wrapper(
	sge::renderer::texture::filter::object const &_value)
:
	value_(
		_value)
{
}

sge::renderer::texture::filter::object const &
fruitcut::pp::texture::filter_wrapper::value() const
{
	return value_;
}

bool
fruitcut::pp::texture::filter_wrapper::operator<(
	filter_wrapper const &r) const
{
	return 
		std::tr1::make_tuple(
			value().min(),
			value().mag(),
			value().anisotropy()) < 
		std::tr1::make_tuple(
			r.value().min(),
			r.value().mag(),
			r.value().anisotropy());
}
