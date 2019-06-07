#include "LuaScript.h"

LuaScript::LuaScript(const std::string & filename)
{
	// Creates and assigns Lua State
	m_L = luaL_newstate();

	if (luaL_loadfile(m_L, filename.c_str()) || lua_pcall(m_L, 0, 0, 0))
	{
		std::cout << "Error: Lua script not loaded (" << filename << ")." << std::endl;

		m_L = nullptr;
	}
}

LuaScript::~LuaScript()
{
	// Closes Lua State on destruction
	if (m_L != nullptr)
		lua_close(m_L);
}

void LuaScript::printError(const std::string & variableName, const std::string & reason)
{
	std::cout << "Error: Can't get [" << variableName << "]. " << reason << std::endl;
}

bool LuaScript::luaGetToStack(const std::string & variableName)
{
	m_level = 0;
	std::string var = "";

	// Cycles through each variable in variable path [variableName] and checks if they're valid
	for (unsigned int i = 0; i < variableName.size(); ++i)
	{
		if (variableName.at(i) != '.')
		{
			var += variableName.at(i);
		}
		else
		{
			if (m_level == 0)
				lua_getglobal(m_L, var.c_str()); // Gets the Lua global, [var] and places it on top of the Lua stack
			else
				lua_getfield(m_L, -1, var.c_str()); // Gets the field variable, [var] of the variable on top of the Lua stack, and places it on top of the stack

				++m_level;

			if (lua_isnil(m_L, -1)) // Check if pushed variable was valid
			{
				printError(variableName, var + " is not defined.");
				return false;
			}
			else
				var = "";
		}
	}

	// Repeat the check once more due to the for loop only checking when there is  a '.'
	if (m_level == 0)
		lua_getglobal(m_L, var.c_str());
	else
		lua_getfield(m_L, -1, var.c_str()); 

	++m_level;

	if (lua_isnil(m_L, -1))
	{
		printError(variableName, var + " is not defined.");
		return false;
	}

	return true;
}
