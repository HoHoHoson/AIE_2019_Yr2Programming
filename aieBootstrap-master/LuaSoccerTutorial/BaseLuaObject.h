#pragma once

#include <lua.hpp>

class BaseLuaObject
{
public:

	/*
		Constructor creates the Lua state and loads the default Lua libraries. 
		Each BaseLuaObject will have its own Lua environment.
	*/
	BaseLuaObject();
	~BaseLuaObject();
	
	/*
		@brief Loads in a Lua script from a provided file path and calls its start function
		@param File path to the script
		@return True if the script was loaded and initialised, else False
	*/
	bool loadScript(const char* szPath);

	/*
		@brief Binds a C++ function to so that it can be called by the Lua script
		@param A name that is assigned to the function binded to the script
		@param The C++ function
	*/
	void registerLuaFunction(const char* szFuncName, lua_CFunction fcnFunction);

	/*
		@brief Calls a function that was added to the Lua stack
		@param How many function arguements that were added to the stack
		@param How many results from the function to place onto the Lua stack   
		@return True if it was successful, else False
	*/
	bool callFunction(int argCount, int returnCount);

	/*
		@brief Passes a pointer containing C++ data to the Lua script
		@param Pointer to the Lua state
		@param Global name assigned to the pointer
		@param Pointer to a C++ variable
	*/
	static void setPointerVar(lua_State* pState, const char* pVarName, void* vpVal);

	/*
		@brief Retrieves a pointer from the Lua script
		@param Pointer to the Lua state
		@param Global name that was assigned to the pointer
		@return The variable pointer address
	*/
	static void* getPointerVar(lua_State* pState, const char* pVarName);

	/*
		@brief Pushes a float variable from the C++ script to the Lua stack to be used
		@param Float value
	*/
	void pushFloat(float fvalue);

	/*
		@brief Pops a float variable off the Lua stack
		@return Float value that was popped off the Lua stack
	*/
	float popFloat();

	/*
		@brief Pushes onto the Lua stack the value of the global name
		@param Global name of the function
	*/
	void pushFunction(const char* szFunction);

private:

	lua_State* m_pLua_state;
};