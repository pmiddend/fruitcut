#ifndef FRUITLIB_RECTANGLE_MANAGER_OBJECT_DECL_HPP_INCLUDED
#define FRUITLIB_RECTANGLE_MANAGER_OBJECT_DECL_HPP_INCLUDED

#include <fruitlib/rectangle_manager/object_fwd.hpp>
#include <fruitlib/rectangle_manager/padding.hpp>
#include <fruitlib/rectangle_manager/rectangle_instance_fwd.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/math/box/object_impl.hpp>
#include <fcppt/math/box/rect.hpp>
#include <fcppt/math/dim/object_impl.hpp>
#include <fcppt/math/vector/object_impl.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/chrono/duration.hpp>
#include <boost/ptr_container/ptr_list.hpp>
#include <fcppt/config/external_end.hpp>


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
	boost::chrono::duration<value_type>
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

#endif