#include "LuaScript.h"
#include "TestClass.h"

void luaTest(lua_State* L)
{
	lua_getglobal(L, "test");

	if (lua_pcall(L, 1, 0, 0))
		std::cout << "Error: can't run Lua function \"test\", lua_pcall error - " << lua_tostring(L, -1) << std::endl;
}

int main()
{
	LuaScript lua_script;
	lua_script.loadScript("../scripts/lua_script.lua");

	registerTestClass(lua_script.getState());
	luaTest(lua_script.getState());

	std::cout << "Stack count = " << lua_gettop(lua_script.getState()) << std::endl; //Stack count checker

	std::cout << lua_script.get<float>("struct.tab.inside") << std::endl;
	std::cout << lua_script.get<std::string>("struct.words") << std::endl;
	std::cout << lua_script.get<int>("struct.words") << std::endl;

	std::vector<std::string> test_vec(lua_script.getVector<std::string>("struct.intvector"));

	for (std::string i : test_vec)
		std::cout << i << std::endl;

	for (std::string s : lua_script.getTableKeys("struct"))
		std::cout << s << std::endl;

	for (std::string s : lua_script.getTableKeys("struct.tab.inside"))
		std::cout << s << std::endl;

	std::cout << "Press ENTER to close window...";
	getchar();
	return 0;
}