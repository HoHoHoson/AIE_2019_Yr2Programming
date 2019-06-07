#include "LuaScript.h"

int main()
{
	LuaScript lua_script("../scripts/lua_script.lua");

	std::cout << lua_script.get<float>("struct.words") << std::endl;

	return 0;
}