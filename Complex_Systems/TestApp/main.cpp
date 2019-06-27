#include "LuaScript.h"
#include "TestClass.h"

void luaFunFunct(lua_State* L)
{
	lua_getglobal(L, "funFunct");

	if (lua_pcall(L, 0, 0, 0))
		std::cout << "Error: can't run Lua function \"funFunct\", lua_pcall error - " << lua_tostring(L, -1) << std::endl;
}

int main()
{
	LuaScript lua_script;

	std::cout << "From C++ : Attempting to load \"lua_script.lua\"" << std::endl;
	lua_script.loadScript("../scripts/lua_script.lua");

	registerTestClass(lua_script.getState());
	std::cout << "\nFrom C++ : C++ class [TestClass] has been binded to Lua\n" << std::endl;

	std::cout << "From C++ : Calling Lua Binder's [getTableKeys(\"tableA\")] returns -> " << std::endl;
	for (std::string s : lua_script.getTableKeys("tableA"))
		std::cout << s << std::endl;

	std::cout << "\nFrom C++ : Calling Lua Binder's [get<float>(\"tableA.my_float\")] returns -> "<< lua_script.get<float>("tableA.my_float") << std::endl;
	std::cout << "From C++ : Calling Lua Binder's [get<std::string>(\"tableA.my_string\")] returns -> " << lua_script.get<std::string>("tableA.my_string") << std::endl;

	std::cout << "\nFrom C++ : Calling Lua Binder's [getVector<std::string>(\"tableA.my_int_array\")] returns -> " << std::endl;
	for (std::string i : lua_script.getVector<std::string>("tableA.my_int_array"))
		std::cout << i << std::endl;

	std::cout << "\nFrom C++ : Calling \"lua_script.lua\" global function, [funFunct]" << std::endl;
	luaFunFunct(lua_script.getState());

	std::cout << "\nPress ENTER to close window...";
	getchar();
	return 0;
}