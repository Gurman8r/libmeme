#include <libmeme/Engine/Script.hpp>
#include <libmeme/Engine/Lua.hpp>
#include <libmeme/Engine/Python.hpp>
#include "..\..\..\include\libmeme\Engine\Script.hpp"

namespace ml
{
	Script::Script()
	{
	}
	Script::Script(std::string const & filename)
	{
	}
	Script::Script(Script const & copy)
	{
	}
	Script::Script(Script && copy) noexcept
	{
	}
	Script::~Script()
	{
	}
	Script & Script::operator=(Script const & other) noexcept
	{
		return (*this);
	}
	Script & Script::operator=(Script && other) noexcept
	{
		return (*this);
	}
	void Script::swap(Script & other)
	{
	}
	bool Script::loadFromFile(std::string const & filename)
	{
		return false;
	}
}