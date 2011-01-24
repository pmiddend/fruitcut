#ifndef FRUITCUT_PP_FILTER_BASE_HPP_INCLUDED
#define FRUITCUT_PP_FILTER_BASE_HPP_INCLUDED

#include "../texture/counted_instance.hpp"
// We friendify pp::system, but pp::system is in another namespace, so
// it needs a FUCKING forward declaration, wtf?
#include "../system_fwd.hpp"
#include <vector>

namespace fruitcut
{
namespace pp
{
namespace filter
{
class base
{
public:
	explicit
	base();

	virtual 
	~base();
private:
	friend class fruitcut::pp::system;

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
