#ifndef FRUITLIB_RECTANGLE_MANAGER_RECTANGLE_INSTANCE_DECL_HPP_INCLUDED
#define FRUITLIB_RECTANGLE_MANAGER_RECTANGLE_INSTANCE_DECL_HPP_INCLUDED

#include "object_fwd.hpp"
#include <fcppt/math/box/box.hpp>
#include <fcppt/math/vector/vector.hpp>

namespace fruitlib
{
namespace rectangle_manager
{
template<typename T>
class rectangle_instance
{
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
	object<T>
	manager_object;

	explicit
	rectangle_instance(
		manager_object &,
		dim const &);

	rect const &
	bounds() const;

	void
	pos(
		vector const &);

	void
	target(
		vector const &);

	vector const &
	target() const;

	void
	kill();

	value_type
	status_fraction() const;

	void
	status_fraction(
		value_type);

	bool
	killed() const;

	~rectangle_instance();
private:
	manager_object &manager_;
	rect bounds_;
	vector target_;
	value_type status_fraction_;
	bool killed_;
};
}
}

#endif
