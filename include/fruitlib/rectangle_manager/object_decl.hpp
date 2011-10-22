#ifndef FRUITLIB_RECTANGLE_MANAGER_OBJECT_DECL_HPP_INCLUDED
#define FRUITLIB_RECTANGLE_MANAGER_OBJECT_DECL_HPP_INCLUDED

#include <fruitlib/rectangle_manager/object_fwd.hpp>
#include <fruitlib/rectangle_manager/padding.hpp>
#include <fruitlib/rectangle_manager/rectangle_instance_fwd.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/chrono/duration_fwd.hpp>
#include <fcppt/math/box/basic_impl.hpp>
#include <fcppt/math/box/rect.hpp>
#include <fcppt/math/dim/basic_impl.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/ptr_container/ptr_list.hpp>
#include <fcppt/config/external_end.hpp>


namespace fruitlib
{
namespace rectangle_manager
{
/*
	Abstract:

	The rectangle manager object tries to manage a "stack" of rectangles
	inside a given rectangular area (inside a "bounding
	box"). Rectangles come and go. If a rectangle is inserted, it slides
	in smoothly from one side of the bounding box. If a rectangle in the
	middle of the stack is deleted, the other rectangles will slide down
	to fill the gap.

	Note that the class has nothing to do with _rendering_
	rectangles. It just aligns them.

	The coordinate type used for the rectangle, the positions and
	dimensions, is specified as a template parameter. It's supposed to
	be floating point, but that's not explicitly forced here (didn't
	want to pollute the class with an enable_if.

	In the constructor, you pass the bounding rectangle, the speed of
	the moving rectangles and the "padding". Tha padding class specifies
	the padding on the left, top and inbetween the rectangles. Padding
	is the same as in HTML: You specify a rectangle, but the content of
	this rectangle is not always supposed to fill the whole
	rectangle. You want to leave a small margin at the borders, so it
	looks better.

	Rectangles are represented by the rectangle_instance class. The user
	is supposed to instantiate objects of this class and pass the
	rectangle manager in the constructor. When a rectangle_instance is
	destroyed, it will be removed from the manager.

	Implementation:

	Internally, each rectangle has a size, a position and a target
	position. When you create a rectangle, the manager will give it an
	initial position and a target. The initial position will be "left of
	the bounding rectangle" at the end of the rectangle stack. The
	initial target will be "somewhere right of the initial position", so
	it will slide in.

	You can kill rectangles, which is just a way of resetting the target
	position to "somewhere left of the bounding rectangle", nothing
	more. It doesn't destroy the rectangle objects.

	Similarly, reviving rectangles just means giving them a new target
	at the end of the stack.

	To simplify calculations, two containers are used to store the
	rectangles: One for the killed rectangles and one for the living
	ones. Killing and reviving rectangles involves moving them from one
	container to the other.
 */
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
		value_type acceleration);

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
	value_type const acceleration_;
	instance_sequence instances_;
	instance_sequence dead_instances_;

	void
	insert(
		instance &);

	void
	kill(
		instance const &);

	void
	revive(
		instance &);

	void
	erase(
		instance const &);

	void
	update_kinematics(
		duration const &,
		instance &);

	void
	insert_impl(
		instance &,
		bool set_position);
};
}
}

#endif
