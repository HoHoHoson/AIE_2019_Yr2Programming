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

	LuaScript();
	~LuaScript();

	lua_State* getState() { return m_L; }

	/**
		@brief Loads a Lua script from the
		@param specified filepath
	*/
	void loadScript(const std::string& filePath);

	/**
		Cleans out the lua_State and loads back in default loaded states, 
		manually loaded in scripts must be reloaded by the user
	*/
	void reloadLuaState();

	/*
		@brief Gets a variable from the Lua script
		@param Variable wanted from the script
		@return The intended variable, default if failed
	*/
	template<typename T>
	T get(const std::string& variable_name);

	/*
		@brief Gets an array from the Lua script
		@param Name of the array in the Lua script
		@return The specified array if it succeeds, otherwise, an empty array 
	*/
	template<typename T>
	std::vector<T> getVector(const std::string& vector_name);

	/*
		@brief Gets all the keys of the variables in 
		@param [table_name]
		@return A string vector that contains the table's keys or an empty vector if the function fails fails
	*/
	std::vector<std::string> getTableKeys(const std::string& table_name);

private:

	/*
		@brief Custom error printer
		@param The name/description of the variable
		@param was not returned due to [reason]
	*/
	void printError(const std::string& variable_name, const std::string& reason);

	/*
		Loads Lua functions that I have typed up in c++ string format, should only be loaded once per lua_State
	*/
	void loadStringCode();

	/*
		Clears the Lua stack, should be done at the end of a function that is done with the Lua stack
	*/
	void luaClearStack();

	/*
	@brief Places a Lua variable on top of the Lua stack
	@param Global name, [variableName], assigned to variable
	@returns True on success, False otherwise
*/
	bool luaGetToStack(const std::string& variable_name);

	/*
		@brief Checks and gets the variable on top of the Lua stack
		@param Name/Description of the Lua variable
		@returns Either the top stack variable or if it fails, the default value for the variable type
	*/
	template<typename T>
	T luaGet(const std::string& variable_name) { return 0; }

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
inline bool LuaScript::luaGet(const std::string& variable_name)
{
	return (bool)lua_toboolean(m_L, -1);
}

template<>
inline int LuaScript::luaGet(const std::string& variable_name)
{
	if (lua_isnumber(m_L, -1) == false)
		printError(variable_name, "not an int");

	return (int)lua_tonumber(m_L, -1);
}

template<>
inline float LuaScript::luaGet(const std::string& variable_name)
{
	if (lua_isnumber(m_L, -1) == false)
		printError(variable_name, "not a float");

	return (float)lua_tonumber(m_L, -1);
}

template<>
inline std::string LuaScript::luaGet(const std::string& variable_name)
{
	std::string str = "null";

	if (lua_isstring(m_L, -1) == false)
		printError(variable_name, "not a string");
	else
		str = lua_tostring(m_L, -1);

	return str;
}


template<typename T>
inline T LuaScript::get(const std::string & variable_name)
{
	T result;

	if (luaGetToStack(variable_name)) // Checks to see if variable exists, places it on stack if it does
		result = luaGet<T>(variable_name); // Gets the variable on the top of the stack
	else
		result = luaGetDefault<T>(); // Gets the default value of the variable if it fails

	luaClearStack();
	return result;
}


template<typename T>
inline std::vector<T> LuaScript::getVector(const std::string & vector_name)
{
	std::vector<T> vector;

	if (luaGetToStack(vector_name))
	{
		lua_pushnil(m_L);
		// Stack should now contain the array and a nil value, marks the start of the array for lua_next
		unsigned int vector_index = 0; // Keep track of iteration for troubleshooting
		while (lua_next(m_L, -2)) // Takes in the array and current key, pops off the key and pushes on the next key and it's value, returns 0 when at the end
		{
			std::string value_desc = vector_name + "[" + std::to_string(vector_index) + "]";
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


#endif // !LUASCRIPT_H
