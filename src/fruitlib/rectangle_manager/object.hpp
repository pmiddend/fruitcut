#ifndef FRUITCUT_FRUITLIB_RECTANGLE_MANAGER_OBJECT_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_RECTANGLE_MANAGER_OBJECT_HPP_INCLUDED

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
	rectangle_manager);
public:
	typedef typename
	fcppt::math::box::rect<T>::type
	rect;

	typedef typename
	rect::vector
	vector;

	explicit
	rectangle_manager(
		rect const &_bounding_rect)
	:
		bounding_rect_(
			_bounding_rect)
	{
	}

	
private:
	class rectangle_instance
	{
	public:
		explicit
		rectangle_instance(
			rectangle_manager::state::type const _state,
			rect const &_bounds)
		:
			state_(
				_state),
			bounds_(
				_bounds)
		{
		}

		rect const &
		bounds() const
		{
			return bounds_;
		}

		void
		bounds(
			rect const &_bounds)
		{
			bounds_ = 
				_bounds;
		}

		rectangle_manager::state::type const
		state() const
		{
			return state_;
		}

		void
		state(
			rectangle_manager::state::type const _state)
		{
			state_ = 
				_state;
		}

		void
		target(
			vector const &_target)
		{
			target_ = 
				_target;
		}
	private:
		rectangle_manager::state::type state_;
		rect bounds_;
		vector target_;
	};

	typedef
	boost::ptr_list<rectangle_instance>
	instance_sequence;

	rect const bounding_rect_;
	instance_sequence instances_;
};
}
}
}

#endif
