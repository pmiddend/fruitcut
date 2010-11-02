#include "variable.hpp"
#include <fcppt/string.hpp>
#include <fcppt/text.hpp>
#include <fcppt/variant/object_impl.hpp>
#include <fcppt/lexical_cast.hpp>
#include <fcppt/variant/apply_unary.hpp>
#include <fcppt/format.hpp>
#include <fcppt/io/ostringstream.hpp>
#include <utility>

namespace
{
template<typename Source>
sge::renderer::glsl::string const
lexical_cast(
	Source const &s)
{
	fcppt::io::ostringstream ss;
	ss << std::showpoint << s;
	return ss.str();
}

class type_value_generator
{
public:
	typedef 
	std::pair
	<
		sge::renderer::glsl::string,
		sge::renderer::glsl::string
	> 
	result_type;

	result_type
	operator()(
		fruitcut::scalar const &v) const
	{
		return result_type(FCPPT_TEXT("float"),"("+lexical_cast(v)+")");
	}

	result_type
	operator()(
		int const v) const
	{
		return result_type(FCPPT_TEXT("int"),"("+lexical_cast(v)+")");
	}

	result_type
	operator()(
		fruitcut::vec2 const &v) const
	{
		return result_type(FCPPT_TEXT("vec2"),lexical_cast(v));
	}

	result_type
	operator()(
		fruitcut::vec3 const &v) const
	{
		return result_type(FCPPT_TEXT("vec3"),lexical_cast(v));
	}

	result_type
	operator()(
		fruitcut::vec4 const &v) const
	{
		return result_type(FCPPT_TEXT("vec4"),lexical_cast(v));
	}

	result_type
	operator()(
		fruitcut::mat4 const &v) const 
	{
		return result_type(FCPPT_TEXT("mat4"),lexical_cast(v));
	}
};

sge::renderer::glsl::string const
generate_declaration(
	sge::renderer::glsl::string const &name,
	fruitcut::graphics::shader::variable_type::type const t,
	fruitcut::graphics::shader::value_type const &v)
{
	sge::renderer::glsl::string s;

	switch (t)
	{
		case fruitcut::graphics::shader::variable_type::uniform:
			s = FCPPT_TEXT("uniform ");
			break;
		case fruitcut::graphics::shader::variable_type::const_:
			s = FCPPT_TEXT("const ");
			break;
	}

	std::pair<sge::renderer::glsl::string,sge::renderer::glsl::string> const type_value = 
		fcppt::variant::apply_unary(
			type_value_generator(),
			v);

	s += type_value.first + FCPPT_TEXT(" ") + name;

	switch (t)
	{
		case fruitcut::graphics::shader::variable_type::uniform:
			s += FCPPT_TEXT(";");
			break;
		case fruitcut::graphics::shader::variable_type::const_:
			s += 
				(fcppt::format(FCPPT_TEXT(" = %s%s;"))
					% type_value.first
					% type_value.second).str();
			break;
	}

	return s;
}
}

fruitcut::graphics::shader::variable::variable(
	sge::renderer::glsl::string const &_name,
	variable_type::type const _type,
	value_type const &_initial_value)
:
	name_(
		_name),
	declaration_(
		generate_declaration(
			_name,
			_type,
			_initial_value)),
	type_(
		_type),
	initial_value_(
		_initial_value)
{
}

sge::renderer::glsl::string const
fruitcut::graphics::shader::variable::declaration() const
{
	return declaration_;
}

fruitcut::graphics::shader::variable_type::type
fruitcut::graphics::shader::variable::type() const
{
	return type_;
}

sge::renderer::glsl::string const
fruitcut::graphics::shader::variable::name() const
{
	return name_;
}

fruitcut::graphics::shader::value_type const
fruitcut::graphics::shader::variable::initial_value() const
{
	return initial_value_;
}
