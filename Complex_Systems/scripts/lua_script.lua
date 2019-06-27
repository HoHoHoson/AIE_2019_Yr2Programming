tableA =
{
	my_string = "This is a string within tableA";
	my_float = 1.05;
	my_int_array = {1, 6, 9, 16};
	tableB = { tabB_string = "This string is inside TableB, which is inside TableA"; };
	["SixNine"] = 69;
};

function funFunct()

print("From Lua : [funFunct] successfully called")
print("From Lua : [funFunct] start")

print("From Lua : [funFunct] created instance of C++ class \"TestClass\" with a variable of 0, within Lua")
newClass = TestClass(0);
print("From Lua : [funFunct] calling TestClass.get() returns -> "..newClass:get());

print("From Lua : [funFunct] calling TestClass.set(123)");
newClass:set(123);
print("From Lua : [funFunct] calling TestClass.get() returns -> "..newClass:get());

print("From Lua : [funFunct] end")

end

print("From Lua : \"lua_script.lua\" has been successfully loaded");