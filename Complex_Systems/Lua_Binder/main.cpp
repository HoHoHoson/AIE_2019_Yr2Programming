#include "LuaScript.h"

void write(const std::string& wordsToWrite)
{
	std::cout << wordsToWrite << std::endl;
}

static int l_write(lua_State* L) 
{
	const char* s = lua_tostring(L, -1); // get function argument
	write(s); // calling C++ function
	return 0; // nothing to return
}

float multiply(float x, float y)
{
	return x * y;
}

static int l_multiply(lua_State* L)
{
	float x = lua_tonumber(L, -2);
	float y = lua_tonumber(L, -1);

	lua_pushnumber(L, multiply(x, y));

	return 1;
}

void callLuaSum(LuaScript& script, int x, int y)
{
	if (script.isNull())
	{
		std::cout << "Script not loaded." << std::endl;
		return;
	}

	std::string func_name = "sum";

	if (lua_getglobal(script.getState(), func_name.c_str()))
	{
		lua_pushnumber(script.getState(), x);
		lua_pushnumber(script.getState(), y);

		lua_pcall(script.getState(), 2, 1, 0);

		std::cout << x << " + " << y << " = " << lua_tonumber(script.getState(), -1) << std::endl;
		lua_pop(script.getState(), 1);
	}
	else
		std::cout << "Can't load function." << func_name << std::endl;
}

int main()
{
	LuaScript lua_script("../scripts/lua_script.lua");

	//std::cout << lua_script.get<float>("struct.tab.inside") << std::endl;
	//std::cout << lua_script.get<std::string>("struct.words") << std::endl;
	//std::cout << lua_script.get<int>("struct.words") << std::endl;

	//std::vector<std::string> test_vec(lua_script.getVector<std::string>("struct.intvector"));

	//for (std::string i : test_vec)
	//{
	//	std::cout << i << std::endl;
	//}

	//callLuaSum(lua_script, 6, 9);

	//for (std::string s : lua_script.getTableKeys("struct"))
	//	std::cout << s << std::endl;

	//if (!lua_script.isNull())
	//{
	//	lua_register(lua_script.getState(), "write", l_write);
	//	lua_register(lua_script.getState(), "multiply", l_multiply);

	//	lua_getglobal(lua_script.getState(), "test");
	//	lua_pcall(lua_script.getState(), 0, 0, NULL);

	//	std::cout << "Stack count = " << lua_gettop(lua_script.getState()) << std::endl;
	//}

	for (std::string s : lua_script.getTableKeys("struct.tab.inside"))
		std::cout << s << std::endl;

	std::cout << "Press ENTER to close window...";
	getchar();
	return 0;
}