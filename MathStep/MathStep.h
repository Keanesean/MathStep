#pragma once
#include <iostream>
#include <queue>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

//Token structure store the tokenized pieces of the equation.  Used to keep track of operators and numbers in order to show the steps in solving.
struct token
{
	unsigned int index;
	std::string value;
};
class MathStep
{
public:
	MathStep();
	~MathStep();

	std::vector<std::string> Calculate(std::string input);
	void PrintSteps();
private:
	void RemoveSpaces();
	bool SyntaxTokenizer();
	void RemoveRedundantParenthesis();
	void PostFix();
	void CalculateSteps();

	bool IsNumber(std::string);

	std::string equation;
	std::vector<token> equationTokenized;
	std::vector<token> equationPostfix;
	std::vector<std::string> equationStep;
};

