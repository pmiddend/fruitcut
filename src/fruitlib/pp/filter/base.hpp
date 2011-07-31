#ifndef FRUITLIB_PP_FILTER_BASE_HPP_INCLUDED
#define FRUITLIB_PP_FILTER_BASE_HPP_INCLUDED

#include "../texture/counted_instance.hpp"
#include "../system_fwd.hpp"
#include <fcppt/noncopyable.hpp>
#include <vector>

namespace fruitlib
{
namespace pp
{
namespace filter
{
class base
{
FCPPT_NONCOPYABLE(
	base);
public:
	explicit
	base();

	virtual 
	~base();
private:
	friend class pp::system;

	typedef 
	std::vector<texture::counted_instance> 
	texture_queue;

	texture_queue textures_;

	void
	enqueue(
		texture::counted_instance const &);

	texture::counted_instance const
	dispatch();
};
}
}
}

#endif
