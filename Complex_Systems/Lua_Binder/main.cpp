#include "LuaScript.h"

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

	return 0;
}