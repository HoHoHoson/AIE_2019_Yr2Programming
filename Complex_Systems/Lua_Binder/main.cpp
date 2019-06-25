#include "LuaScript.h"

int main()
{
	LuaScript lua_script;
	lua_script.loadScript("../scripts/lua_script.lua");
	std::cout << "Stack count = " << lua_gettop(lua_script.getState()) << std::endl; //Stack count checker

	lua_getglobal(lua_script.getState(), "test");
	lua_pcall(lua_script.getState(), 0, 0, 0);

	std::cout << lua_script.get<float>("struct.tab.inside") << std::endl;
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

	//for (std::string s : lua_script.getTableKeys("struct.tab.inside"))
	//	std::cout << s << std::endl;

	std::cout << "Press ENTER to close window...";
	getchar();
	return 0;
}