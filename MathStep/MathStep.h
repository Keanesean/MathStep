#pragma once
#include <iostream>
#include <queue>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

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
	void PostFix();
	void CalculateSteps();

	bool IsNumber(std::string);

	std::string equation;
	std::vector<token> equationTokenized;
	std::vector<token> equationPostfix;
	std::vector<std::string> equationStep;
};

