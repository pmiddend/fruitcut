#ifndef FRUITCUT_FRUITLIB_RECTANGLE_MANAGER_OBJECT_DECL_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_RECTANGLE_MANAGER_OBJECT_DECL_HPP_INCLUDED

#include "object_fwd.hpp"
#include "padding.hpp"
#include "rectangle_instance_fwd.hpp"
#include <fcppt/math/box/box.hpp>
#include <fcppt/math/vector/vector.hpp>
#include <fcppt/math/dim/dim.hpp>
#include <fcppt/chrono/duration_fwd.hpp>
#include <fcppt/noncopyable.hpp>
#include <boost/ptr_container/ptr_list.hpp>

namespace fruitcut
{
namespace fruitlib
{
namespace rectangle_manager
{
template<typename T>
class object
{
FCPPT_NONCOPYABLE(
	object);
public:
	typedef
	T
	value_type;

	typedef typename
	fcppt::math::box::rect<value_type>::type
	rect;

	typedef typename
	rect::vector
	vector;

	typedef typename
	rect::dim
	dim;

	typedef
	rectangle_manager::padding<value_type>
	padding;

	typedef
	rectangle_manager::rectangle_instance<value_type>
	instance;

	typedef
	fcppt::chrono::duration<value_type>
	duration;

	explicit
	object(
		rect const &,
		padding const &,
		value_type speed);

	void
	update(
		duration const &);

	~object();
private:
	friend class rectangle_instance<value_type>;

	typedef
	boost::ptr_list<instance,boost::view_clone_allocator>
	instance_sequence;

	typedef typename
	instance_sequence::iterator
	instance_iterator;

	rect const bounding_rect_;
	padding const padding_;
	value_type const speed_;
	instance_sequence instances_;
	instance_sequence dead_instances_;

	void
	insert(
		instance &);

	void
	kill(
		instance const &);

	void
	erase(
		instance const &);
};
}
}
}

#endif
