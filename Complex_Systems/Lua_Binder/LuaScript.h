#ifndef LUASCRIPT_H
#define LUASCRIPT_H

#include <vector>
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
		@brief Gets a variable from the Lua script
		@param Variable wanted from the script
		@return The intended variable, default if failed
	*/
	template<typename T>
	T get(const std::string& variableName) 
	{
		if (m_L == nullptr)
		{
			printError(variableName, "script is not loaded.");
			return luaGetDefault<T>();
		}

		T result;

		if (luaGetToStack(variableName)) // Checks to see if variable exists, places it on stack if it does
			result = luaGet<T>(variableName); // Gets the variable on the top of the stack
		else
			result = luaGetDefault<T>(); // Gets the default value of the variable if it fails

		luaClearStack();
		return result;
	}

	/*
		@brief Gets an array from the Lua script
		@param Name of the array in the Lua script
		@return The specified array if it succeeds, otherwise, an empty array 
	*/
	template<typename T>
	std::vector<T> getVector(const std::string& vectorName)
	{
		std::vector<T> vector;

		if (m_L == nullptr)
		{
			printError(vectorName, "script is not loaded.");
			return vector;
		}

		if (luaGetToStack(vectorName))
		{
			lua_pushnil(m_L);
			// Stack should now contain the array and a nil value, marks the start of the array for lua_next
			unsigned int vector_index = 0; // Keep track of iteration for troubleshooting
			while (lua_next(m_L, -2)) // Takes in the array and current key, pops off the key and pushes on the next key and it's value, returns 0 when at the end
			{
				std::string value_desc = vectorName + "[" + std::to_string(vector_index) + "]";
				T value = luaGet<T>(value_desc);

				if (value == luaGetDefault<T>()) // Checks for type mismatches
				{
					printError(value_desc, "the variable type doesn't match vector type");
					vector.clear();
					break;
				}

				vector.push_back(value); // Adds the gotten value to our vector
				++vector_index;

				lua_pop(m_L, 1); // Pops off the recorded value, only the array and table key is left. Therefore it is now ready for the next iteration
			}
		}

		luaClearStack();
		return vector;
	}

private:

	/*
		@brief Custom error printer
		@param The name/description of the variable
		@param was not returned due to [reason]
	*/
	void printError(const std::string& variableName, const std::string& reason);

	/*
		Clears the Lua stack, should be done at the end of a function that is done with the Lua stack
	*/
	void luaClearStack();

	/*
	@brief Places a Lua variable on top of the Lua stack
	@param Global name, [variableName], assigned to variable
	@returns True on success, False otherwise
*/
	bool luaGetToStack(const std::string& variableName);

	/*
		@brief Checks and gets the variable on top of the Lua stack
		@param Name/Description of the Lua variable
		@returns Either the top stack variable or if it fails, the default value for the variable type
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

	lua_State* m_L;
};

template<>
inline std::string LuaScript::luaGetDefault()
{
	return "null";
}

// luaGet templates
template<>
inline bool LuaScript::luaGet(const std::string& variableName)
{
	return (bool)lua_toboolean(m_L, -1);
}

template<>
inline int LuaScript::luaGet(const std::string& variableName)
{
	if (lua_isnumber(m_L, -1) == false)
		printError(variableName, "not a number.");

	return (int)lua_tonumber(m_L, -1);
}

template<>
inline float LuaScript::luaGet(const std::string& variableName)
{
	if (lua_isnumber(m_L, -1) == false)
		printError(variableName, "not a number.");

	return (float)lua_tonumber(m_L, -1);
}

template<>
inline std::string LuaScript::luaGet(const std::string& variableName)
{
	std::string str = "null";

	if (lua_isstring(m_L, -1) == false)
		printError(variableName, "not a string.");
	else
		str = lua_tostring(m_L, -1);

	return str;
}


#endif // !LUASCRIPT_H

