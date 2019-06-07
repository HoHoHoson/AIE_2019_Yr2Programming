#include "LuaScript.h"

int main()
{
	LuaScript lua_script("../scripts/lua_script.lua");

	std::cout << lua_script.get<float>("struct.tab.inside") << std::endl;
	std::cout << lua_script.get<std::string>("struct.words") << std::endl;

	return 0;
}