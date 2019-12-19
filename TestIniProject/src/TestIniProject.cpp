#include <iostream>
#include <IniParser.h>

int main()
{
	IniParser test{ "Resources/configs/general.ini" };

	std::cout << test.Get<int>("height");
	test.Add<int>("test", 13);
	test.Set("width", "1024");
	//test.Remove("height");
	test.Save();

	return EXIT_SUCCESS;
}