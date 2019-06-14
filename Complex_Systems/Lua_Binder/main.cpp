#include "LuaScript.h"

void sum(LuaScript& script, int x, int y)
{
	if (script.isNull())
		std::cout << "Script not loaded." << std::endl;

	std::string func_name = "sum";

	if (lua_getglobal(script.state(), func_name.c_str()))
	{
		lua_pushnumber(script.state(), x);
		lua_pushnumber(script.state(), y);

		lua_pcall(script.state(), 2, 1, 0);

		std::cout << x << " + " << y << " = " << lua_tonumber(script.state(), -1) << std::endl;
		lua_pop(script.state(), 1);
	}
	else
		std::cout << "Can't load function." << func_name << std::endl;
}

int main()
{
	LuaScript lua_script("../scripts/lua_script.lua");

	std::cout << lua_script.get<float>("struct.tab.inside") << std::endl;
	std::cout << lua_script.get<std::string>("struct.words") << std::endl;
	std::cout << lua_script.get<int>("struct.words") << std::endl;

	std::vector<std::string> test_vec(lua_script.getVector<std::string>("struct.intvector"));

	for (std::string i : test_vec)
	{
		std::cout << i << std::endl;
	}

	sum(lua_script, 6, 9);

	for (std::string s : lua_script.getTableKeys("struct"))
		std::cout << s << std::endl;

	return 0;
}