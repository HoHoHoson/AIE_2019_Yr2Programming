#include "BaseLuaObject.h"
#include <iostream>

BaseLuaObject::BaseLuaObject()
{
	m_pLua_state = luaL_newstate();
	luaL_openlibs(m_pLua_state);

	setPointerVar(m_pLua_state, "self", this);
}

BaseLuaObject::~BaseLuaObject()
{
}

bool BaseLuaObject::loadScript(const char* szPath)
{
	int status = luaL_dofile(m_pLua_state, szPath);
	if (status)
	{
		std::cout << "Couldn't load script -> " << szPath << std::endl << 
			lua_tostring(m_pLua_state, -1);

		return false;
	}

	/// Run the Initialise function in the Lua script
	//  Start by adding the function to the stack
	lua_getglobal(m_pLua_state, "Initialise");
	//  Now execute it
	status = lua_pcall(m_pLua_state, 0, 0, NULL);
	if (status)
	{
		std::cout << "Couldn't run function 'Initialise' -> " <<
			lua_tostring(m_pLua_state, -1) << std::endl;
	}

	return true;
}

void BaseLuaObject::registerLuaFunction(const char * szFuncName, lua_CFunction fcnFunction)
{
	lua_register(m_pLua_state, szFuncName, fcnFunction);
}

bool BaseLuaObject::callFunction(int argCount, int returnCount)
{
	if (!lua_isfunction(m_pLua_state, -(argCount + 1)))
	{
		std::cout << "BaseLuaObject::callFunction -> function is not in the correct position on the stack";

		return false;
	}

	int status = lua_pcall(m_pLua_state, argCount, returnCount, NULL);

	if (status)
	{
		std::cout << "Couldn't run function -> " << lua_tostring(m_pLua_state, -1);

		return false;
	}

	return true;
}

void BaseLuaObject::setPointerVar(lua_State * pState, const char * pVarName, void * vpVal)
{
	lua_pushlightuserdata(pState, vpVal);
	lua_setglobal(pState, pVarName);
}

void * BaseLuaObject::getPointerVar(lua_State * pState, const char * pVarName)
{
	lua_getglobal(pState, pVarName);

	if (lua_isuserdata(pState, -1) == false)
	{
		std::cout << "BaseLuaObject::getPointerVar -> variable is not a pointer";

		return NULL;
	}
	else
	{
		void* vpVal = (void*)lua_topointer(pState, -1);
		lua_pop(pState, 1);

		return vpVal;
	}
}

void BaseLuaObject::pushFloat(float fvalue)
{
	lua_pushnumber(m_pLua_state, fvalue);
}

float BaseLuaObject::popFloat()
{
	if (lua_isnumber(m_pLua_state, -1) == false)
	{
		int iStackSize = lua_gettop(m_pLua_state);
		std::cout << "BaseLuaObject::popFloat - > variable is not a number";

		return 0.0f;
	}
	else
	{
		float fVal = (float)lua_tonumber(m_pLua_state, -1);
		lua_pop(m_pLua_state, 1);

		return fVal;
	}
}

void BaseLuaObject::pushFunction(const char * szFunction)
{
	lua_getglobal(m_pLua_state, szFunction);
	if (lua_isfunction(m_pLua_state, -1) == false)
	{
		std::cout << "BaseLuaObject::pushFunction -> variable is not a function";

		return;
	}
}