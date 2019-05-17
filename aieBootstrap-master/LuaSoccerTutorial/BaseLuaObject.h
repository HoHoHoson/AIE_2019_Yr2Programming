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
		@brief Binds a C++ function to so that it can be called from the Lua script
		@param
		@param
	*/
	void registerLuaFunction(const char* szFuncName, lua_CFunction fcnFunction);
	/*
		@brief Calls a Lua function from our C++ code
		@param
		@param
		@return True if it was successful, else False
	*/
	bool callFunction(int argCount, int returnCount);

	/*
		@brief Passes a pointer to the Lua script
		@param
		@param
	*/
	static void setPointerVar(lua_State* pState, const char* pVarName, void* vpVal);
	/*
		@brief Retrieves a pointer from the Lua script
		@param
		@param
		@return
	*/
	static void* getPointerVar(lua_State* pState, const char* pVarName);

	/*
		@brief Pushes a float variable from the C++ script to the Lua stack to be used
		@param
	*/
	void pushFloat(float fvalue);
	/*
		@brief Pops a float variable off the Lua stack
		@return 
	*/
	float popFloat();

	/*
		@brief Specifies the function we wish to call on the Lua script
		@param
	*/
	void pushFunction(const char* szFunction);

private:

	lua_State* m_pLua_state;
};