#ifndef FRUITSERVER_ASCII_CHAR_TRAITS_HPP_INCLUDED
#define FRUITSERVER_ASCII_CHAR_TRAITS_HPP_INCLUDED

#include <fruitserver/ascii/char.hpp>
#include <string>
#include <algorithm>
#include <cstddef>
#include <cstring>

namespace fruitserver
{
namespace ascii
{
struct char_traits
{
public:
	typedef
	ascii::char_
	char_type;

	typedef
	int
	int_type;

	typedef
	std::streamoff
	off_type;

	typedef
	std::streampos
	pos_type;

	typedef
	std::mbstate_t
	state_type;

	static void
	assign(
		char_traits::char_type &c1, 
		char_traits::char_type const &c2)
	{ 
		c1.value(
			c2.value()); 
	}

	static bool
	eq(
		char_traits::char_type const &c1, 
		char_traits::char_type const &c2)
	{ 
		return c1.value() == c2.value(); 
	}

	static bool
	lt(
		char_traits::char_type const &c1, 
		char_traits::char_type const &c2)
	{ 
		return c1.value() < c2.value(); 
	}

	static int
	compare(
		char_traits::char_type const * const s1, 
		char_traits::char_type const * const s2, 
		std::size_t const n)
	{
		for(
			std::size_t i = 
				0; 
			i < n; 
			++i)
			if(
				char_traits::lt(
					s1[i], 
					s2[i]))
				return -1;
			else if(
				char_traits::lt(
					s2[i], 
					s1[i]))
				return 1;
		return 0;
	}

	static std::size_t
	length(char_type const *s)
	{
		std::size_t i = 
			0;
		while(
			!eq(
				s[i], 
				char_traits::char_type()))
			++i;
		return i;
	}

	static char_traits::char_type const *
	find(
		char_traits::char_type const *s, 
		std::size_t const n, 
		char_traits::char_type const & a)
	{
		for(
			std::size_t i = 
				0; 
			i < n; 
			++i)
			if(
				char_traits::eq(
					s[i], 
					a))
				return s + i;
		return 0;
	}

	static char_traits::char_type*
	move(
		char_traits::char_type *s1, 
		char_traits::char_type const *s2, 
		std::size_t const n)
	{
		return 
			static_cast<char_traits::char_type *>(
				std::memmove(
					s1, 
					s2,
					n));
	}

	static char_traits::char_type*
	copy(
		char_traits::char_type *s1, 
		char_traits::char_type const *s2, 
		std::size_t const n)
	{
		std::copy(
			s2, 
			s2 + n, 
			s1);
		return s1;
	}

	static char_traits::char_type*
	assign(
		char_traits::char_type *s, 
		std::size_t const n, 
		char_traits::char_type const  a)
	{
		std::fill_n(
			s,
			n,
			a);
		return s;
	}

	static char_traits::char_type
	to_char_type(char_traits::int_type const &c)
	{ 
		return 
			char_traits::char_type(
				static_cast<char_traits::char_type::value_type>(
					c)); 
	}

	static char_traits::int_type
	to_int_type(char_traits::char_type const &c)
	{ 
		return 
			static_cast<char_traits::int_type>(
				c.value()); 
	}

	static bool
	eq_int_type(
		char_traits::int_type const &c1, char_traits::int_type const &c2)
	{ 
		return c1 == c2; 
	}

	static char_traits::int_type
	eof()
	{ 
		return 
			static_cast<char_traits::int_type>(
				-1); 
	}

	static char_traits::int_type
	not_eof(char_traits::int_type const &c)
	{ 
		return 
			!char_traits::eq_int_type(
				c, 
				char_traits::eof()) 
			? 
				c 
			: 
				char_traits::to_int_type(
					char_traits::char_type()); }
};
}
}

#endif
