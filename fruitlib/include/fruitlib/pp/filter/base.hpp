#ifndef FRUITLIB_PP_FILTER_BASE_HPP_INCLUDED
#define FRUITLIB_PP_FILTER_BASE_HPP_INCLUDED

#include <fruitlib/pp/system_fwd.hpp>
#include <fruitlib/pp/texture/counted_instance.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/config/external_begin.hpp>
#include <vector>
#include <fcppt/config/external_end.hpp>


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
