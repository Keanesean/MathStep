#include "MathStep.h"

MathStep::MathStep()
{
}
MathStep::~MathStep()
{
}
std::vector<std::string> MathStep::Calculate(std::string input)
{	
	//Clear variables.
	this->equation.clear();
	this->equationTokenized.clear();
	this->equationPostfix.clear();
	this->equationStep.clear();

	this->equation = input;
	this->RemoveSpaces();

	bool validEquation = this->SyntaxTokenizer();
	if (validEquation)
	{
		this->RemoveRedundantParenthesis();
		this->PostFix();
		this->CalculateSteps();
	}
	else
	{
		//Return empty.
	}

	return this->equationStep;
}
void MathStep::PrintSteps()
{
	for (unsigned int i = 0; i < this->equationStep.size(); i++)
	{
		std::cout << this->equationStep[i] << std::endl;
	}
	std::cout << std::endl;
}

// Private //

void MathStep::RemoveSpaces()
{
	for (unsigned int i = 0; i < this->equation.size(); i++)
	{
		if (isspace(this->equation[i]))
		{
			this->equation.erase(i, 1);
			i--;
		}
	}
}
bool MathStep::SyntaxTokenizer()
{
	unsigned int state = 0;
	unsigned int parenthisisCount = 0;

	std::string op = "+-*/";
	std::string numberBuffer = "";
	
	token equationToken;

	if (this->equation.empty())
	{
		return true;
	}
	else
	{
		for (unsigned int i = 0; i < this->equation.size(); i++)
		{
			char value = this->equation[i];

			switch (state)
			{
			case 0:
				if (value == '(')
				{
					state = 0;
					parenthisisCount++;

					equationToken.index = this->equationTokenized.size();
					equationToken.value = "(";
					this->equationTokenized.push_back(equationToken);
				}
				else if (isdigit(value))
				{
					state = 1;

					numberBuffer += value;
				}
				else if (value == '-')
				{
					state = 2;

					numberBuffer += value;
				}
				else
				{
					return false;
				}
				break;
			case 1:
				if (value == '(')
				{
					state = 0;
					parenthisisCount++;

					if (!numberBuffer.empty())
					{
						equationToken.index = this->equationTokenized.size();
						equationToken.value = numberBuffer;
						this->equationTokenized.push_back(equationToken);
						numberBuffer.clear();
					}
					equationToken.index = this->equationTokenized.size();
					equationToken.value = "*";
					this->equationTokenized.push_back(equationToken);

					equationToken.index = this->equationTokenized.size();
					equationToken.value = "(";
					this->equationTokenized.push_back(equationToken);
				}
				else if (isdigit(value))
				{
					state = 1;

					numberBuffer += value;
				}
				else if (value == ')')
				{
					state = 1;

					if (parenthisisCount > 0)
					{
						parenthisisCount--;
					}
					else
					{
						return false;
					}

					if (!numberBuffer.empty())
					{
						equationToken.index = this->equationTokenized.size();
						equationToken.value = numberBuffer;
						this->equationTokenized.push_back(equationToken);
						numberBuffer.clear();
					}

					equationToken.index = this->equationTokenized.size();
					equationToken.value = ")";
					this->equationTokenized.push_back(equationToken);
				}
				else if (op.find(value) != std::string::npos)
				{
					state = 3;

					if (!numberBuffer.empty())
					{
						equationToken.index = this->equationTokenized.size();
						equationToken.value = numberBuffer;
						this->equationTokenized.push_back(equationToken);
						numberBuffer.clear();
					}
					equationToken.index = this->equationTokenized.size();
					equationToken.value = value;
					this->equationTokenized.push_back(equationToken);
				}
				else
				{
					return false;
				}
				break;
			case 2:
				if (value == '(')
				{
					state = 0;
					parenthisisCount++;

					equationToken.index = this->equationTokenized.size();
					equationToken.value = numberBuffer + "1";
					this->equationTokenized.push_back(equationToken);
					numberBuffer.clear();

					equationToken.index = this->equationTokenized.size();
					equationToken.value = "*";
					this->equationTokenized.push_back(equationToken);

					equationToken.index = this->equationTokenized.size();
					equationToken.value = "(";
					this->equationTokenized.push_back(equationToken);
				}
				else if (isdigit(value))
				{
					state = 1;

					numberBuffer += value;
				}
				else
				{
					return false;
				}
				break;
			case 3:
				if (isdigit(value))
				{
					state = 1;

					numberBuffer += value;
				}
				else if (value == '(')
				{
					state = 1;
					parenthisisCount++;

					equationToken.index = this->equationTokenized.size();
					equationToken.value = "(";
					this->equationTokenized.push_back(equationToken);
				}
				else if (value == '-')
				{
					state = 2;

					numberBuffer += "-";
				}
				else
				{
					return false;
				}
				break;
			}
		}
	}

	if (parenthisisCount == 0 && state == 1)
	{
		if (!numberBuffer.empty())
		{
			equationToken.index = this->equationTokenized.size();
			equationToken.value = numberBuffer;
			this->equationTokenized.push_back(equationToken);
			numberBuffer.clear();
		}

		return true;
	}
	else
	{
		return false;
	}
}
void MathStep::RemoveRedundantParenthesis()
{
	for (unsigned int i = 1; i < this->equationTokenized.size() - 1; i++)
	{
		if (this->equationTokenized[i - 1].value == "(" && this->equationTokenized[i + 1].value == ")")
		{
			this->equationTokenized.erase(this->equationTokenized.begin() + i + 1, this->equationTokenized.begin() + i + 2);
			this->equationTokenized.erase(this->equationTokenized.begin() + i - 1, this->equationTokenized.begin() + i);

			break;
		}
	}
}
void MathStep::PostFix()
{
	std::queue<token> numberQueue;
	std::stack<token> operatorStack;
	std::unordered_map<char, unsigned int> operatorLevel;
	std::unordered_map<char, unsigned int>::iterator equationIterator;

	operatorLevel['+'] = 0;
	operatorLevel['-'] = 0;
	operatorLevel['*'] = 1;
	operatorLevel['/'] = 1;
	operatorLevel['^'] = 2;
	operatorLevel['('] = 3;
	operatorLevel[')'] = 3;

	for (unsigned int i = 0; i < this->equationTokenized.size(); i++)
	{
		token value = this->equationTokenized[i];
		bool isNumber = this->IsNumber(value.value);

		if (isNumber)
		{
			numberQueue.push(value);
		}
		else
		{
			if (operatorStack.empty())
			{
				operatorStack.push(value);
			}
			else
			{
				unsigned int stackLevel, valueLevel;

				equationIterator = operatorLevel.find(operatorStack.top().value[0]);
				stackLevel = equationIterator->second;

				equationIterator = operatorLevel.find(value.value[0]);
				valueLevel = equationIterator->second;
				
				if (value.value == "(")
				{
					operatorStack.push(value);
				}
				else if (value.value == ")")
				{
					while (!numberQueue.empty())
					{
						this->equationPostfix.push_back(numberQueue.front());
						numberQueue.pop();
					}
					while (operatorStack.top().value != "(")
					{
						this->equationPostfix.push_back(operatorStack.top());
						operatorStack.pop();
					}
					operatorStack.pop();
				}
				else if (valueLevel <= stackLevel && operatorStack.top().value != "(")
				{
					while (!numberQueue.empty())
					{
						this->equationPostfix.push_back(numberQueue.front());
						numberQueue.pop();
					}

					while (valueLevel <= stackLevel && !operatorStack.empty() && operatorStack.top().value != "(")
					{				
						this->equationPostfix.push_back(operatorStack.top());
						operatorStack.pop();

						if (operatorStack.empty())
						{
							break;
						}
						equationIterator = operatorLevel.find(operatorStack.top().value[0]);
						stackLevel = equationIterator->second;
					}
					operatorStack.push(value);
				}
				else
				{
					operatorStack.push(value);
				}
			}
		}
	}

	while (!numberQueue.empty())
	{
		this->equationPostfix.push_back(numberQueue.front());
		numberQueue.pop();
	}
	while (!operatorStack.empty())
	{
		this->equationPostfix.push_back(operatorStack.top());
		operatorStack.pop();
	}
}
void MathStep::CalculateSteps()
{
	unsigned int tokenIndex = this->equationTokenized.size() + 1;
	token first, second, computation;
	std::stack<token> stack;
	
	this->equationStep.push_back(this->equation);

	for (unsigned int i = 0; i < this->equationPostfix.size(); i++)
	{
		token value = this->equationPostfix[i];
		bool isNumber = this->IsNumber(value.value);

		if (isNumber)
		{
			stack.push(value);
		}
		else
		{
			second = stack.top();
			stack.pop();
			first = stack.top();
			stack.pop();

			switch (value.value[0])
			{
			case '+':
				computation.index = tokenIndex; tokenIndex++;
				computation.value = std::to_string(stod(first.value) + stod(second.value));
				stack.push(computation);
				break;
			case '-':
				computation.index = tokenIndex; tokenIndex++;
				computation.value = std::to_string(stod(first.value) - stod(second.value));
				stack.push(computation);
				break;
			case '*':
				computation.index = tokenIndex; tokenIndex++;
				computation.value = std::to_string(stod(first.value) * stod(second.value));
				stack.push(computation);
				break;
			case '/':
				computation.index = tokenIndex; tokenIndex++;
				computation.value = std::to_string(stod(first.value) / stod(second.value));
				stack.push(computation);
				break;
			case '^':
				computation.index = tokenIndex; tokenIndex++;
				computation.value = std::to_string(std::pow(stod(first.value), stod(second.value)));
				stack.push(computation);
				break;
			}

			for (unsigned int i = 0; i < this->equationTokenized.size(); i++)
			{
				if (this->equationTokenized[i].index == first.index)
				{
					this->equationTokenized.erase(this->equationTokenized.begin() + i, this->equationTokenized.begin() + i + 3);
					this->equationTokenized.insert(this->equationTokenized.begin() + i, computation);

					if (i > 0)
					{
						if (this->equationTokenized[i - 1].value == "(" && this->equationTokenized[i + 1].value == ")")
						{
							this->equationTokenized.erase(this->equationTokenized.begin() + i - 1, this->equationTokenized.begin() + i);
							this->equationTokenized.erase(this->equationTokenized.begin() + i, this->equationTokenized.begin() + i + 1);
						}
					}
					break;
				}
			}

			std::string step = "";
			for (unsigned int i = 0; i < this->equationTokenized.size(); i++)
			{
				step += this->equationTokenized[i].value;
			}
			this->equationStep.push_back(step);
		}
	}
}

bool MathStep::IsNumber(std::string input)
{
	bool valid;

	if (input[0] == '-' && input.size() == 1)
	{
		return false;
	}
	if (input[0] == '-' || isdigit(input[0]))
	{
		for (unsigned int i = 1; i < input.size(); i++)
		{
			valid = isdigit(input[i]);

			if (!valid)
			{
				return false;
			}
		}
	}
	else
	{
		return false;
	}

	return true;
}