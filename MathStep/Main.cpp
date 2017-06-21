#include <iostream>

#include "MathStep.h"

int main()
{
	MathStep mathStep;
	std::string input;

	do
	{
		std::cout << "Enter equation (Exit to quit): ";
		std::cin >> input;

		if (input == "Exit")
		{
			break;
		}

		mathStep.Calculate(input);
		mathStep.PrintSteps();
	} while (true);
	return 0;
}