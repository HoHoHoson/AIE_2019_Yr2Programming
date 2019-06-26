#include "LuaScript.h"

#include<sstream>

LuaScript::LuaScript()
{
	// Creates and assigns Lua State
	m_L = luaL_newstate();

	luaL_openlibs(m_L); // Load Lua libraries
	loadStringCode(); // Load the Lua functions that were made in c++ string
}

LuaScript::~LuaScript()
{
	// Closes Lua State on destruction
	lua_close(m_L);
}

void LuaScript::loadScript(const std::string & file_path)
{
	if (luaL_loadfile(m_L, file_path.c_str())) // Attempts to load the script at [filepath] onto the Lua stack, it needs to be ran by lua_pcall before it can be used
	{
		std::cout << "Error: " << lua_tostring(m_L, -1) << std::endl; // Print error message on script loading failure
	}
	else if (lua_pcall(m_L, 0, 0, NULL)) // Calls the script that was placed on the stack. It runs the script and/or makes its variables and functions available for use
		printError(file_path, "lua_pcall failure - " + (std::string)lua_tostring(m_L, -1));

	luaClearStack();
}

std::vector<std::string> LuaScript::getTableKeys(const std::string & table_name)
{
	std::vector<std::string> vec;

	if (luaGetToStack(table_name))
	{
		lua_getglobal(m_L, "tableKeys");		 // tableKeys function returns a string that contains all the keys of a specified table from the Lua script
		lua_pushstring(m_L, table_name.c_str()); // Push the path to the table variable onto the Lua stack

		if (lua_pcall(m_L, 1, 1, 0))
			printError(table_name, "lua_pcall failure - " + (std::string)lua_tostring(m_L, -1));
		else
		{
			std::istringstream token_stream(lua_tostring(m_L, -1));

			if (std::strcmp(token_stream.str().c_str(), "null") != 0)
			{
				// Tokenise the chain of keys into individual strings and pushes them into a vector
				std::string token;

				while (std::getline(token_stream, token, ','))
					vec.push_back(token);
			}
			else
				printError(table_name + " [table keys]", "variable is not a table"); // Print an error if the specified path didn't lead to a table
		}
	}

	luaClearStack();
	return vec;
}

void LuaScript::printError(const std::string & variable_name, const std::string & reason)
{
	std::cout << "Error: can't get (" << variable_name << "), " << reason << "." << std::endl;
}

void LuaScript::loadStringCode()
{
	std::string function_code =
		"function tableKeys(name) "

		"	is_global = true;"
		"	var = nil;"

		"	for field in string.gmatch(name, \"%P+\") do "
		"		if (is_global == true) "
		"		then "
		"			var = _G[field];"
		"			is_global = false;"
		"		else "
		"			var = var[field] "
		"		end;"
		"	end;"

		"	if (type(var) ~= \"table\") then return \"null\" end;"

		"	str = \"\";"
		"	for k, v in pairs(var) do "
		"		str = str..k..\",\";"
		"	end;"
		"	return str;"

		"end";

	if (luaL_loadstring(m_L, function_code.c_str()))
	{
		printError("tableKeys", lua_tostring(m_L, -1));
	}
	else if (lua_pcall(m_L, 0, 0, 0))
	{
		printError("tableKeys", "lua_pcall failure - " + (std::string)lua_tostring(m_L, -1));
	}
}

void LuaScript::luaClearStack()
{
	lua_settop(m_L, 0);
}

bool LuaScript::luaGetToStack(const std::string & variable_name)
{
	bool is_global = true;
	std::string token_string = variable_name + "."; // Attaching the '.' at the end will let the for loop check the last variable, due to it only checking once it reaches a '.'
	std::string var = "";

	// Cycles through each variable in variable path [variableName] and checks if they're valid
	for (unsigned int i = 0; i < token_string.size(); ++i)
	{
		if (token_string.at(i) != '.')
		{
			var += token_string.at(i);
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
				printError(variable_name, var + " is nil");
				return false;
			}
			else
				var = "";
		}
	}

	return true;
}
