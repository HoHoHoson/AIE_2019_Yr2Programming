#pragma once
#define LUA_TESTCLASS "TestClass" // Set global name for this class in Lua

class TestClass
{
public:

	TestClass(int i) : m_integer(i) {}
	~TestClass() {}

	int get() { return m_integer; }
	void set(int newi) { m_integer = newi; }

private:

	int m_integer;
};

static int testClass_new(lua_State* L)
{
	// Error checking for the correct number and the right variable types of the arguements passed to the stack
	bool is_valid = true;
	if (lua_gettop(L) == 1)
	{
		if (lua_isnumber(L, 1) == 0)
		{
			std::cout << "Error: can't call testClass_new, arguement 1 is not a number." << std::endl;
			is_valid = false;
		}
	}
	else
	{
		std::cout << "Error: can't call testClass_new, incorrect number of arguements." << std::endl;
		is_valid = false;
	}

	if (!is_valid)
	{
		lua_pushnil(L);
		return 1;
	}

	TestClass** userdata = static_cast<TestClass**>(lua_newuserdata(L, sizeof(TestClass))); // Creates and instantiates 
	(*userdata) = new TestClass((int)lua_tonumber(L, 1));								    // a new TestClass to the Lua stack

	luaL_setmetatable(L, LUA_TESTCLASS); // Sets newly created TestClass obj to use a metatable registered as [LUA_TESTCLASS]
										 // table was created in the "registerTestClass" function

	return 1; // Returns a newly made TestClass
}

static int testClass_delete(lua_State* L)
{
	if (lua_gettop(L) != 1)
		std::cout << "Error: can't call testClass_delete, incorrect number of arguements." << std::endl;

	// Deletes the TestClass instance once Lua's garbage collector gets it
	TestClass** userdata = static_cast<TestClass**>(luaL_checkudata(L, 1, LUA_TESTCLASS));
	delete (*userdata);

	return 0; // Nothing to return
}

static int testClass_get(lua_State* L)
{
	if (lua_gettop(L) != 1)
	{
		std::cout << "Error: can't call testClass_get, incorrect number of arguements." << std::endl;
		return 0;
	}

	// Calls TestClass's "get" function
	TestClass** userdata = static_cast<TestClass**>(luaL_checkudata(L, 1, LUA_TESTCLASS));
	lua_pushnumber(L, (*userdata)->get());

	return 1; // Returns an integer
}

static int testClass_set(lua_State* L)
{
	if (lua_gettop(L) == 2)
	{
		if (lua_isnumber(L, 2) != 1)
		{
			std::cout << "Error: can't call testClass_set, arguement 1 is not a number." << std::endl;
			return 0;
		}
	}
	else
	{
		std::cout << "Error: can't call testClass_set, incorrect number of arguements." << std::endl;
		return 0;
	}

	// Calls TestClass's "set" function
	TestClass** userdata = static_cast<TestClass**>(luaL_checkudata(L, 1, LUA_TESTCLASS));
	(*userdata)->set((int)lua_tonumber(L, 2)); // Sets TestClass's value to a number pushed onto the stack

	return 0; // Returns nothing
}

// Registers the C++ object to be recognised by Lua
void registerTestClass(lua_State* L)
{
	lua_register(L, LUA_TESTCLASS, testClass_new); // Sets TestClass constructor as a new Lua global named [LUA_TESTCLASS]
	luaL_newmetatable(L, LUA_TESTCLASS);		   // Creates a new metatable in the registry named [LUA_TESTCLASS] and pushes it to stack for binding/use

	lua_pushcfunction(L, testClass_delete); // Pushing on TestClass destructor
	lua_setfield(L, -2, "__gc");			// which gets assigned to the __gc (garbage collector) metamethod of the new metatable

	lua_pushvalue(L, -1);					// Creates a new copy of the table on top of the stack
	lua_setfield(L, -2, "__index");			// then sets its as the __index, allows class methods to be binded and called from Lua

	lua_pushcfunction(L, testClass_get); // Binds function testClass_get to the metatable
	lua_setfield(L, -2, "get");

	lua_pushcfunction(L, testClass_set); // Binds function testClass_set to the metatable
	lua_setfield(L, -2, "set");

	lua_pop(L, 1); // Pops off the metatable once binds are complete
}