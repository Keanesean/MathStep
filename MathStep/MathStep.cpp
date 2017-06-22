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

	//Get the equation and remove white spaces.
	this->equation = input;
	this->RemoveSpaces();

	//Parse the equationa to check its syntax, at the same time tokenize it.
	bool validEquation = this->SyntaxTokenizer();
	if (validEquation)
	{
		//Remove redundant parenthesis, copy equation into postfix notation, and calculate the steps.
		this->RemoveRedundantParenthesis();
		this->PostFix();
		this->CalculateSteps();
	}
	else
	{
		//Invalid quation.  Clear the variables just in case.
		this->equation.clear();
		this->equationTokenized.clear();
		this->equationPostfix.clear();
		this->equationStep.clear();
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

/////////////
// Private //
/////////////

/*
	Remove all spaces from the input string.  Allows users to enter equation however they like so long as it is syntactically correct.
*/
void MathStep::RemoveSpaces()
{
	for (unsigned int i = 0; i < this->equation.size(); i++)
	{
		//If the character is a space.
		if (isspace(this->equation[i]))
		{
			//Delete the character.  
			this->equation.erase(i, 1);
			i--;
		}
	}
}
/*
	Returns boolean if equation syntax is correct or incorrect.
	Uses string of equation with no spaces.  Parses through the string checking state and valid state changes.  Need to add DPDA to program for reference.
	While parsing the equation, as well tokenize the equation into a vector.
*/
bool MathStep::SyntaxTokenizer()
{
	//Current state.  ParenthesisCount can never be negative, if it was to occur the equation is invalid.  ( => ++, ) => --.
	unsigned int state = 0;
	unsigned int parenthesisCount = 0;

	//Check to see if decimal.
	bool decimal = false;

	//Currently abailable operators.
	std::string op = "+-*/^";

	//Buffers numbers that are multiline or negative.
	std::string numberBuffer = "";
	
	//Temp storage.  Holds the value which is a operator or number.  Holds the index of that value as well in order to distinguish from other numbers/operators.
	token equationToken;

	//Return true if empty, technically a correct equation...
	if (this->equation.empty())
	{
		return true;
	}
	else
	{
		//Loop through the equation string.
		for (unsigned int i = 0; i < this->equation.size(); i++)
		{
			char value = this->equation[i];

			//Switch control for state.  Default is state 0.  State 4 is checked after the switch control.
			//If in a state and the next character is not defined, then the character is invalid and the equation is therefore invalid.
			switch (state)
			{
			case 0:
				if (value == '(')
				{
					state = 0;
					parenthesisCount++;

					equationToken.index = this->equationTokenized.size();
					equationToken.value = "(";
					this->equationTokenized.push_back(equationToken);
				}
				else if (isdigit(value))
				{
					state = 1;

					numberBuffer += value;
				}
				else if (value == '.' && decimal == false)
				{
					state = 1;

					numberBuffer += value;
					decimal = true;
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
					parenthesisCount++;

					if (!numberBuffer.empty())
					{
						if (numberBuffer == ".")
						{
							return false;
						}

						equationToken.index = this->equationTokenized.size();
						equationToken.value = numberBuffer;
						this->equationTokenized.push_back(equationToken);
						numberBuffer.clear();
						decimal = false;
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
				else if (value == '.' && decimal == false)
				{
					state = 1;

					numberBuffer += value;
					decimal = true;
				}
				else if (value == ')')
				{
					state = 1;

					if (parenthesisCount > 0)
					{
						parenthesisCount--;
					}
					else
					{
						return false;
					}

					if (!numberBuffer.empty())
					{
						if (numberBuffer == ".")
						{
							return false;
						}

						equationToken.index = this->equationTokenized.size();
						equationToken.value = numberBuffer;
						this->equationTokenized.push_back(equationToken);
						numberBuffer.clear();
						decimal = false;
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
						if (numberBuffer == ".")
						{
							return false;
						}

						equationToken.index = this->equationTokenized.size();
						equationToken.value = numberBuffer;
						this->equationTokenized.push_back(equationToken);
						numberBuffer.clear();
						decimal = false;
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
					parenthesisCount++;

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
				else if (value == '.' && decimal == false)
				{
					state = 1;

					numberBuffer += value;
					decimal = true;
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
				else if (value == '.' && decimal == false)
				{
					state = 1;

					numberBuffer += value;
					decimal = true;
				}
				else if (value == '(')
				{
					state = 1;
					parenthesisCount++;

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

	if (parenthesisCount == 0 && state == 1)
	{
		if (!numberBuffer.empty())
		{
			if (numberBuffer == ".")
			{
				return false;
			}

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
/*
	Simply goes through the tokenized buffer and removes cases where a single number has an opening and closing parenthesis on its side.
	No point in keeping those parenthesis and causes display issues in CalculateSteps.
*/
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
/*
	Converts to postifx notation.  Used to calculate without need of parenthesis.
*/
void MathStep::PostFix()
{
	std::queue<token> numberQueue;
	std::stack<token> operatorStack;
	std::unordered_map<char, unsigned int> operatorLevel;
	std::unordered_map<char, unsigned int>::iterator equationIterator;

	//Operator level, used to check in operatorStack what to do.
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
/*
	Solves the equation as well as pushes each step into a vector.
*/
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

			//Logic to create new equation after each step.
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
/*
	Simply checks if input is a number.  Cannot use isdigit as it only take a character.  
	Handles negative numbers. Handles decimal numbers.
*/
bool MathStep::IsNumber(std::string input)
{
	bool valid;
	bool decimal = false;

	if (input[0] == '-' && input.size() == 1)
	{
		return false;
	}
	if (input[0] == '-' || isdigit(input[0]) || input[0] == '.')
	{
		for (unsigned int i = 1; i < input.size(); i++)
		{
			valid = isdigit(input[i]);

			if (input[i] == '.' && decimal == false)
			{
				valid = true;
			}

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