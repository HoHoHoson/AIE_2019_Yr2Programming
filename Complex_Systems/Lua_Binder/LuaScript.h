#ifndef LUASCRIPT_H
#define LUASCRIPT_H

#include <string>
#include <iostream>

// Lua libraries are written in C, therefore we have to let the compiler know
extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

// Note: Lua Stack Index
// -1 is the object on the top of the stack, -2 is the top second, -3 is the top third, -n is the top n etc.

// Each instance of LuaScript has their own lua_State
class LuaScript
{
public:

	LuaScript(const std::string& filename);
	~LuaScript();

	/*
		@brief Custom error printer
		@param Prints an error message that tells how [variableName]
		@param was not returned due to [reason]
	*/
	void printError(const std::string& variableName, const std::string& reason);

	/*
		@brief Gets a variable from the Lua script
		@param Variable wanted from the script
		@return The intended variable, default if failed
	*/
	template<typename T>
	T get(const std::string& variableName) 
	{
		if (m_L == nullptr)
		{
			printError(variableName, "Script is not loaded.");
			return luaGetDefault<T>();
		}

		T result;

		if (luaGetToStack(variableName)) // Checks to see if variable exists, places it on stack if it does
			result = luaGet<T>(variableName); // Gets the variable on the top of the stack
		else
			result = luaGetDefault<T>(); // Gets the default value of the variable if it fails

		lua_pop(m_L, m_level); // Clear Lua stack after finished
		return result;
	}

	/*
		@brief Places a Lua variable on top of the Lua stack
		@param Global name, [variableName], assigned to variable
		@returns True on success, False otherwise
	*/
	bool luaGetToStack(const std::string& variableName);

	/*
		@brief Gets the variable on top of the Lua stack
		@param Name of the Lua script variable
		@returns The intended variable or default of variable type if failed
	*/
	template<typename T>
	T luaGet(const std::string& variableName) { return 0; }

	/*
		@brief Gets the default value
		@param of variable assigned to global name, [variableName]
		@returns nullptr, 0, "null" etc. Depends on variable type
	*/
	template<typename T>
	T luaGetDefault() { return 0; }

private:

	lua_State* m_L;

	int m_level; // Keeps track of the depth of the current variable and the number of objects on the Lua stack
};



template<>
inline std::string LuaScript::luaGetDefault()
{
	return "null";
}



template<>
inline bool LuaScript::luaGet(const std::string& variableName)
{
	return (bool)lua_toboolean(m_L, -1);
}

template<>
inline int LuaScript::luaGet(const std::string& variableName)
{
	if (lua_isnumber(m_L, -1) == false)
		printError(variableName, "Not a number.");

	return (int)lua_tonumber(m_L, -1);
}

template<>
inline float LuaScript::luaGet(const std::string& variableName)
{
	if (lua_isnumber(m_L, -1) == false)
		printError(variableName, "Not a number.");

	return (float)lua_tonumber(m_L, -1);
}

template<>
inline std::string LuaScript::luaGet(const std::string& variableName)
{
	std::string str = "null";

	if (lua_isstring(m_L, -1) == false)
		printError(variableName, "Not a string.");
	else
		str = lua_tostring(m_L, -1);

	return str;
}



#endif // !LUASCRIPT_H
