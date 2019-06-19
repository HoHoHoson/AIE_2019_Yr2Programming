#include "LuaScript.h"

LuaScript::LuaScript(const std::string & filename)
{
	// Creates and assigns Lua State
	m_L = luaL_newstate();

	if (luaL_loadfile(m_L, filename.c_str())) // Attempts to load the script at [filepath] onto the Lua stack, it needs to be ran by lua_pcall before it can be used
	{
		std::cout << "Error: " << lua_tostring(m_L, -1) << std::endl; // Print error message on script loading failure

		lua_close(m_L);
		m_L = nullptr;
	}
	else
	{
		luaL_openlibs(m_L); // Load Lua libraries

		if (lua_pcall(m_L, 0, 0, NULL) != 0) // Calls the script that was placed on the stack. It runs the script and/or makes its variables and functions available for use
			std::cout << "Error: " << lua_tostring(m_L, -1) << std::endl;
	}
}

LuaScript::~LuaScript()
{
	// Closes Lua State on destruction
	if (m_L != nullptr)
		lua_close(m_L);
}

bool LuaScript::isNull()
{
	return (m_L == nullptr);
}

std::vector<std::string> LuaScript::getTableKeys(const std::string & table_name)
{
	std::vector<std::string> vec;

	if (isNull())
	{
		printError(table_name, "script is not loaded");
		return vec;
	}

	if (luaGetToStack(table_name))
	{
		//lua_pushnil(m_L);

		//while (lua_next(m_L, -2))
		//{
		//	lua_pop(m_L, 1);
		//	vec.push_back(lua_tostring(m_L, -1));
		//}

		std::string function_code =
			"function codefunc(name) "

			"is_global = true;"
			"table = nil;"
			"s = \"struct.tab\";"

			"for w in string.gmatch(s, \"%P+\") do "

			"	if (is_global == true) "
			"	then "
			"		table = _G[w] print(\"then\") is_global = false "
			"	else "
			"		table = table[w] print(\"else\") "
			"	end;"

			"	print(table.inside) "

			"end;"

			"end";

		if (luaL_loadstring(m_L, function_code.c_str()) != 0)
			std::cout << lua_tostring(m_L, -1) << std::endl;

		lua_pcall(m_L, 0, 0, NULL);

		std::string s = "12 biubi fiwhf id 69";

		lua_getglobal(m_L, "codefunc");
		lua_pushstring(m_L, s.c_str());
		lua_pcall(m_L, 1, 0, 0);

		//std::cout << "codefunc returned: " << lua_tostring(m_L, -1) << std::endl;
	}

	luaClearStack();
	return vec;
}

void LuaScript::printError(const std::string & variable_name, const std::string & reason)
{
	std::cout << "Error: can't get (" << variable_name << "), " << reason << "." << std::endl;
}

void LuaScript::luaClearStack()
{
	int stack_count = lua_gettop(m_L);

	lua_pop(m_L, stack_count);
}

bool LuaScript::luaGetToStack(const std::string & variable_name)
{
	bool is_global = true;
	std::string var = "";

	// Cycles through each variable in variable path [variableName] and checks if they're valid
	for (unsigned int i = 0; i < variable_name.size(); ++i)
	{
		if (variable_name.at(i) != '.')
		{
			var += variable_name.at(i);
		}
		else
		{
			if (is_global)
			{
				lua_getglobal(m_L, var.c_str()); // Gets the Lua global, [var] and places it on top of the Lua stack
				is_global = false;
			}
			else
				lua_getfield(m_L, -1, var.c_str()); // Gets the field variable, [var] of the variable on top of the Lua stack, and places it on top of the stack

			if (lua_isnil(m_L, -1)) // Check if pushed variable was valid
			{
				printError(variable_name, var + " is not defined");
				return false;
			}
			else
				var = "";
		}
	}

	// Repeat the check once more due to the for loop only checking when there is  a '.'
	if (is_global)
		lua_getglobal(m_L, var.c_str());
	else
		lua_getfield(m_L, -1, var.c_str()); 

	if (lua_isnil(m_L, -1))
	{
		printError(variable_name, var + " is not defined");
		return false;
	}

	return true;
}
