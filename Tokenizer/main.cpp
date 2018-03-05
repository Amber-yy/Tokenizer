#include "Tokenizer.h"

#include <iostream>

int main()
{
	try
	{
		Tokenizer t;
		t.parse(std::string("+"));
	}
	catch (std::string &e)
	{
		std::cout << e;
	}

	return 0;
}