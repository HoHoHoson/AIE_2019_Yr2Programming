struct =
{
	number = 69;
	words = "this is a string";
	tab = { inside = 11; };
	intvector = {"s", "t", "r", "i", "n", "g", 69},
	["gy8g"] = 69
};

function sum(x, y)
return (x + y);
end

function test()

newClass = TestClass(123);
print(newClass:get());

newClass:set(69);
print(newClass:get());

end