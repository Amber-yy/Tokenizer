#include "Tokenizer.h"

#include <vector>
#include <map>
#include <functional>
#include <iostream>

struct Tokenizer::Data
{
	std::vector<Token> allToken;
	std::map<char, std::function<void(Tokenizer *)>> method;
	std::string code;
	int currentIndex;
	int codeIndex;
	int currentLine;
};

Tokenizer::Tokenizer()
{
	data = new Data;

	data->method.insert(std::pair<char, std::function<void(Tokenizer *)>>('+', &Tokenizer::getAdd));
	data->method.insert(std::pair<char, std::function<void(Tokenizer *)>>('-', &Tokenizer::getSub));
	data->method.insert(std::pair<char, std::function<void(Tokenizer *)>>('*', &Tokenizer::getMulti));
	data->method.insert(std::pair<char, std::function<void(Tokenizer *)>>('/', &Tokenizer::getDivi));
	data->method.insert(std::pair<char, std::function<void(Tokenizer *)>>('%', &Tokenizer::getMod));
	data->method.insert(std::pair<char, std::function<void(Tokenizer *)>>('=', &Tokenizer::getAssign));
	data->method.insert(std::pair<char, std::function<void(Tokenizer *)>>('~', &Tokenizer::getBitNegation));
	data->method.insert(std::pair<char, std::function<void(Tokenizer *)>>('^', &Tokenizer::getBitXor));
	data->method.insert(std::pair<char, std::function<void(Tokenizer *)>>(',', &Tokenizer::getComma));
	data->method.insert(std::pair<char, std::function<void(Tokenizer *)>>('!', &Tokenizer::getNegation));
	data->method.insert(std::pair<char, std::function<void(Tokenizer *)>>('>', &Tokenizer::getGreater));
	data->method.insert(std::pair<char, std::function<void(Tokenizer *)>>('<', &Tokenizer::getLess));
	data->method.insert(std::pair<char, std::function<void(Tokenizer *)>>('&', &Tokenizer::getAnd));
	data->method.insert(std::pair<char, std::function<void(Tokenizer *)>>('|', &Tokenizer::getOr));
	data->method.insert(std::pair<char, std::function<void(Tokenizer *)>>('(', &Tokenizer::getLeftBracket));
	data->method.insert(std::pair<char, std::function<void(Tokenizer *)>>(')', &Tokenizer::getRightBracket));
	data->method.insert(std::pair<char, std::function<void(Tokenizer *)>>('[', &Tokenizer::getLeftSqBracket));
	data->method.insert(std::pair<char, std::function<void(Tokenizer *)>>(']', &Tokenizer::getRightSqBracket));
	data->method.insert(std::pair<char, std::function<void(Tokenizer *)>>('{', &Tokenizer::getLeftCurBracket));
	data->method.insert(std::pair<char, std::function<void(Tokenizer *)>>('}', &Tokenizer::getRightCurBracket));
	data->method.insert(std::pair<char, std::function<void(Tokenizer *)>>('?', &Tokenizer::getConditionLeft));
	data->method.insert(std::pair<char, std::function<void(Tokenizer *)>>(':', &Tokenizer::getConditionRight));
	data->method.insert(std::pair<char, std::function<void(Tokenizer *)>>('\n', &Tokenizer::getEnter));
	data->method.insert(std::pair<char, std::function<void(Tokenizer *)>>(';', &Tokenizer::getEndOfState));
}

Tokenizer::~Tokenizer()
{
	delete data;
}

void Tokenizer::addError(std::string & info)
{
	throw std::to_string(data->currentLine) + info;
}

void Tokenizer::parse(std::string & code)
{
	data->code = std::move(code);
	data->allToken.clear();
	data->currentIndex = 0;
	data->codeIndex = 0;
	data->currentLine = 1;

	while (true)
	{
		char temp;

		do
		{
			temp = peekChar();
			if (!isSpace(temp))
			{
				break;
			}
			getChar();
		} while (true);

		if (temp == -1)
		{
			break;
		}

		if (temp == '/'&&(peekChar(1)=='/'||peekChar(1)=='*'))
		{
			getComments();
		}
		else if (isNumber(temp))
		{
			getNumber();
		}
		else if (isIdStart(temp))
		{
			getId();
		}
		else if (isQuotes(temp))
		{
			getStringLiteral();
		}
		else
		{
			auto it = data->method.find(temp);
			if (it == data->method.end())
			{
				addError(std::string("无法识别的标识符"));
			}
			else
			{
				it->second(this);
			}
		}
	}
}

void Tokenizer::getEndOfState()
{
	getChar();

	Token t;
	t.setLine(data->currentLine);
	t.addType(EndOfState);
	t.setString(std::string(";"));
	data->allToken.push_back(std::move(t));
}

void Tokenizer::getBitXor()
{
	getChar();

	Token t;
	t.setLine(data->currentLine);
	t.addType(Operator);

	char next = peekChar();
	if (next == '=')
	{
		getChar();
		t.setOperator(BitOrAssign);
		t.setString(std::string("^="));
		data->allToken.push_back(std::move(t));
		return;
	}

	t.setOperator(BitXor);
	t.setString(std::string("^"));
	data->allToken.push_back(std::move(t));
}

void Tokenizer::getBitNegation()
{
	getChar();

	Token t;
	t.setLine(data->currentLine);
	t.addType(Operator);
	t.setOperator(BitNegation);
	t.setString(std::string("~"));
	data->allToken.push_back(std::move(t));
}

void Tokenizer::getEnter()
{
	getChar();

	Token t;
	t.setLine(data->currentLine);
	t.addType(EndOfLine);
	t.setString(std::string("\n"));
	data->allToken.push_back(std::move(t));
}

void Tokenizer::getConditionLeft()
{
	getChar();

	Token t;
	t.setLine(data->currentLine);
	t.addType(Operator);
	t.setOperator(ConditionLeft);
	t.setString(std::string("?"));
	data->allToken.push_back(std::move(t));
}

void Tokenizer::getConditionRight()
{
	getChar();

	Token t;
	t.setLine(data->currentLine);
	t.addType(Operator);
	t.setOperator(ConditionRight);
	t.setString(std::string(":"));
	data->allToken.push_back(std::move(t));
}

void Tokenizer::getLeftCurBracket()
{
	getChar();

	Token t;
	t.setLine(data->currentLine);
	t.addType(Operator);
	t.setOperator(LeftSqBracket);
	t.setString(std::string("{"));
	data->allToken.push_back(std::move(t));
}

void Tokenizer::getRightCurBracket()
{
	getChar();

	Token t;
	t.setLine(data->currentLine);
	t.addType(Operator);
	t.setOperator(RightSqBracket);
	t.setString(std::string("}"));
	data->allToken.push_back(std::move(t));
}

void Tokenizer::getLeftSqBracket()
{
	getChar();

	Token t;
	t.setLine(data->currentLine);
	t.addType(Operator);
	t.setOperator(LeftSqBracket);
	t.setString(std::string("["));
	data->allToken.push_back(std::move(t));
}

void Tokenizer::getRightSqBracket()
{
	getChar();

	Token t;
	t.setLine(data->currentLine);
	t.addType(Operator);
	t.setOperator(RightSqBracket);
	t.setString(std::string("]"));
	data->allToken.push_back(std::move(t));
}

void Tokenizer::getLeftBracket()
{
	getChar();

	Token t;
	t.setLine(data->currentLine);
	t.addType(Operator);
	t.setOperator(LeftBracket);
	t.setString(std::string("("));
	data->allToken.push_back(std::move(t));
}

void Tokenizer::getRightBracket()
{
	getChar();

	Token t;
	t.setLine(data->currentLine);
	t.addType(Operator);
	t.setOperator(RightBracket);
	t.setString(std::string(")"));
	data->allToken.push_back(std::move(t));
}

void Tokenizer::getOr()
{
	getChar();

	Token t;
	t.setLine(data->currentLine);
	t.addType(Operator);

	char next = peekChar();
	if (next == '|')
	{
		getChar();
		t.setOperator(Or);
		t.setString(std::string("||"));
		data->allToken.push_back(std::move(t));
		return;
	}

	t.setOperator(BitOr);
	t.setString(std::string("|"));
	data->allToken.push_back(std::move(t));
}

void Tokenizer::getAnd()
{
	getChar();

	Token t;
	t.setLine(data->currentLine);
	t.addType(Operator);

	char next = peekChar();
	if (next == '&')
	{
		getChar();
		t.setOperator(And);
		t.setString(std::string("&&"));
		data->allToken.push_back(std::move(t));
		return;
	}

	if (isPre())
	{
		t.setOperator(GetAddr);
	}
	else
	{
		t.setOperator(BitAnd);
	}

	t.setString(std::string("&"));
	data->allToken.push_back(std::move(t));
}

void Tokenizer::getGreater()
{
	getChar();

	Token t;
	t.setLine(data->currentLine);
	t.addType(Operator);

	char next = peekChar();
	if (next == '=')
	{
		getChar();
		t.setOperator(GreaterEqual);
		t.setString(std::string(">="));
		data->allToken.push_back(std::move(t));
		return;
	}

	if (next == '>')
	{
		getChar();
		if (peekChar() == '=')
		{
			getChar();
			t.setOperator(RightMoveAssign);
			t.setString(std::string(">>="));
			data->allToken.push_back(std::move(t));
			return;
		}
		t.setOperator(RightMove);
		t.setString(std::string(">>"));
		data->allToken.push_back(std::move(t));
		return;
	}

	t.setOperator(Greater);
	t.setString(std::string(">"));
	data->allToken.push_back(std::move(t));
}

void Tokenizer::getLess()
{
	getChar();

	Token t;
	t.setLine(data->currentLine);
	t.addType(Operator);

	char next = peekChar();
	if (next == '=')
	{
		getChar();
		t.setOperator(LessEqual);
		t.setString(std::string("<="));
		data->allToken.push_back(std::move(t));
		return;
	}

	if (next == '<')
	{
		getChar();
		if (peekChar() == '=')
		{
			getChar();
			t.setOperator(LeftMoveAssign);
			t.setString(std::string("<<="));
			data->allToken.push_back(std::move(t));
			return;
		}
		t.setOperator(LeftMove);
		t.setString(std::string("<<"));
		data->allToken.push_back(std::move(t));
		return;
	}

	t.setOperator(Less);
	t.setString(std::string("<"));
	data->allToken.push_back(std::move(t));
}

void Tokenizer::getNegation()
{
	getChar();

	Token t;
	t.setLine(data->currentLine);
	t.addType(Operator);

	char next = peekChar();
	if (next == '=')
	{
		getChar();
		t.setOperator(NotEqual);
		t.setString(std::string("!="));
		data->allToken.push_back(std::move(t));
		return;
	}

	t.setOperator(Negation);

	t.setString(std::string("!"));
	data->allToken.push_back(std::move(t));
}

void Tokenizer::getComma()
{
	getChar();

	Token t;
	t.setLine(data->currentLine);
	t.addType(Operator);
	t.setOperator(Comma);
	t.setString(std::string(","));
	data->allToken.push_back(std::move(t));
}

void Tokenizer::getMember()
{
	getChar();

	Token t;
	t.setLine(data->currentLine);
	t.addType(Operator);
	t.setOperator(Member);
	t.setString(std::string("."));
	data->allToken.push_back(std::move(t));
}

void Tokenizer::getAssign()
{
	getChar();

	Token t;
	t.setLine(data->currentLine);
	t.addType(Operator);

	char next = peekChar();
	if (next == '=')
	{
		getChar();
		t.setOperator(Equal);
		t.setString(std::string("=="));
		data->allToken.push_back(std::move(t));
		return;
	}

	t.setOperator(Assign);

	t.setString(std::string("="));
	data->allToken.push_back(std::move(t));
}

void Tokenizer::getMod()
{
	getChar();

	Token t;
	t.setLine(data->currentLine);
	t.addType(Operator);

	char next = peekChar();
	if (next == '=')
	{
		getChar();
		t.setOperator(ModAssign);
		t.setString(std::string("%="));
		data->allToken.push_back(std::move(t));
		return;
	}

	t.setOperator(Mod);

	t.setString(std::string("%"));
	data->allToken.push_back(std::move(t));
}

void Tokenizer::getDivi()
{
	getChar();

	Token t;
	t.setLine(data->currentLine);
	t.addType(Operator);

	char next = peekChar();
	if (next == '=')
	{
		getChar();
		t.setOperator(DiviAssign);
		t.setString(std::string("/="));
		data->allToken.push_back(std::move(t));
		return;
	}

	t.setOperator(Multi);

	t.setString(std::string("/"));
	data->allToken.push_back(std::move(t));
}

void Tokenizer::getMulti()
{
	getChar();

	Token t;
	t.setLine(data->currentLine);
	t.addType(Operator);

	char next = peekChar();
	if (next == '=')
	{
		getChar();
		t.setOperator(MultiAssign);
		t.setString(std::string("*="));
		data->allToken.push_back(std::move(t));
		return;
	}

	if (isPre())
	{
		t.setOperator(GetValue);
	}
	else
	{
		t.setOperator(Multi);
	}

	t.setString(std::string("*"));
	data->allToken.push_back(std::move(t));
}

void Tokenizer::getSub()
{
	getChar();

	Token t;
	t.setLine(data->currentLine);
	t.addType(Operator);

	char next = peekChar();
	if (next == '=')
	{
		getChar();
		t.setOperator(SubAssign);
		t.setString(std::string("-="));
		data->allToken.push_back(std::move(t));
		return;
	}

	if (next == '-')
	{
		getChar();
		if (isPre())
		{
			t.setOperator(PreDecrement);
		}
		else
		{
			t.setOperator(PostDecrement);
		}

		t.setString(std::string("--"));
		data->allToken.push_back(std::move(t));
		return;
	}

	if (next == '>')
	{
		getChar();
		t.setOperator(MemberP);
		t.setString(std::string("->"));
		data->allToken.push_back(std::move(t));
		return;
	}

	if (isPre())
	{
		t.setOperator(Negative);
	}
	else
	{
		t.setOperator(Sub);
	}

	t.setString(std::string("-"));
	data->allToken.push_back(std::move(t));
}

void Tokenizer::getAdd()
{
	getChar();

	Token t;
	t.setLine(data->currentLine);
	t.addType(Operator);

	char next = peekChar();
	if (next == '=')
	{
		getChar();
		t.setOperator(AddAssign);
		t.setString(std::string("+="));
		data->allToken.push_back(std::move(t));
		return;
	}

	if (next == '+')
	{
		getChar();
		if (isPre())
		{
			t.setOperator(PreIncrement);
		}
		else
		{
			t.setOperator(PostIncrement);
		}

		t.setString(std::string("++"));
		data->allToken.push_back(std::move(t));
		return;
	}

	if (isPre())
	{
		t.setOperator(Positive);
	}
	else
	{
		t.setOperator(Add);
	}

	t.setString(std::string("+"));
	data->allToken.push_back(std::move(t));
}

void Tokenizer::getComments()
{
	getChar();

	char t = getChar();
	if (t == '/')//单行注释
	{
		while (true)
		{
			t = getChar();
			if (t == '\n'||t==-1)
			{
				return;
			}
		}
	}
	else if (t == '*')//多行注释
	{
		while (t != -1)
		{
			t = getChar();
			if (t == '*')
			{
				if (getChar() == '/')
				{
					return;
				}
			}
		}

		addError(std::string("错误的注释格式"));
	}
	else
	{
		addError(std::string("错误的注释格式"));
	}
}

void Tokenizer::getNumber()
{
	bool real = false;
	bool isHex = false;
	bool isOct = false;
	bool isDec = true;
	char temp;

	std::string number,origin;

	if (peekChar() == '0')
	{
		temp = peekChar(1);

		if (temp == 'x'||temp=='X')
		{
			getChar();
			getChar();
			isHex = true;
			origin += "0x";
		}
		else if (isNumber(temp))
		{
			getChar();
			isOct = true;
			origin += "0";
		}
		else if(temp!='l'&&temp!='L'&&temp != 'u'&&temp != 'U')
		{
			addError(std::string("错误的数字格式"));
		}

	}

	if (isHex || isOct)
	{
		isDec = false;
	}

	while (isReal(peekChar(),isHex))
	{
		temp = getChar();
		if (temp == '.')
		{
			if (real || !isDec)
			{
				addError(std::string("此处不应有'.'"));
				break;
			}
			else
			{
				real = true;
			}
		}
		
		number += temp;
		origin += temp;
	}

	temp = peekChar();

	if (temp == 'l'||temp == 'L'||temp == 'u'||temp == 'U')
	{
		origin+=getChar();
	}
	else if (temp=='f'||temp=='F')
	{
		if (!real)
		{
			addError(std::string("错误的数字格式"));
		}
		origin += getChar();
	}
	else if (!isDiv(temp))
	{
		addError(std::string("数字字面量之后有不合法的文本运算符"));
	}

	Token t;
	t.setLine(data->currentLine);

	if (real)
	{
		t.addType(RealLiteral);
		t.setReal(atof(number.c_str()));
	}
	else
	{
		t.addType(IntLiteral);
		unsigned long long result;
		if (isHex)
		{
			result = parseHex(number);
		}
		else if (isOct)
		{
			result = parseOct(number);
		}
		else
		{
			result = parseDec(number);
		}

		t.setInteger(result);
	}

	t.setString(std::move(origin));
	data->allToken.push_back(std::move(t));
}

void Tokenizer::getId()
{
	std::string id;

	while (isId(peekChar()))
	{
		id += getChar();
	}

	Token t;
	t.setLine(data->currentLine);
	t.setString(id);
	t.addType(Identifier);

	if (isKeyword(id))
	{
		t.addType(KeyWord);
		if (id == "sizeof")
		{
			t.addType(Operator);
		}
	}

	data->allToken.push_back(std::move(t));
}

void Tokenizer::getStringLiteral()
{
	int quotes = isQuotes(peekChar());
	Token token;
	token.setLine(data->currentLine);
	getChar();

	if (quotes == 1)
	{
		unsigned long long t=0;
		while (true)
		{
			unsigned long long temp=getChar();

			if (temp == '\'')
			{
				token.addType(IntLiteral);
				token.setInteger(t);
				data->allToken.push_back(std::move(token));
				return;
			}

			if (temp == -1||temp=='\n')
			{
				break;
			}

			if (temp == '\\')
			{
				temp = getChar();
				if (temp == -1)
				{
					break;
				}

				unsigned long long c = parseTran(temp);
				if (c != -1)
				{
					temp = c;
				}
			}

			t *= 256;
			t += temp;
		}

		addError(std::string("应输入'\''"));
	}

	std::string str;

	while (true)
	{
		char temp = getChar();

		if (temp == -1||'\n')
		{
			break;
		}

		if (temp == '\"')//相连的两个字面量字符串视为同一个
		{
			while (true)
			{
				if (!isSpace(peekChar()))
				{
					break;
				}
				getChar();
			}

			if (peekChar() == '\"')
			{
				getChar();
				continue;
			}
			else
			{
				token.addType(StringLiteral);
				token.setString(str);
				data->allToken.push_back(std::move(token));
				return;
			}
		}

		if (temp == '\\')
		{
			temp = getChar();
			if (temp == -1)
			{
				break;
			}
			else if (temp == '\n')
			{
				continue;
			}
			else
			{
				char c = parseTran(temp);
				if (c != -1)
				{
					temp = c;
				}
			}
		}

		str += temp;
	}

	addError(std::string("应输入'\"'"));
}

void Tokenizer::unGetChar()
{
	--data->codeIndex;
}

const Token & Tokenizer::read()
{
	++data->currentIndex;
	if (data->currentIndex >= data->allToken.size())
	{
		addError(std::string("代码不完整，缺少必要的Token"));
	}
	return data->allToken[data->currentIndex - 1];
}

const Token & Tokenizer::peek(int index)
{
	if (data->currentIndex + index >= data->allToken.size())
	{
		addError(std::string("代码不完整，缺少必要的Token"));
	}
	return data->allToken[data->currentIndex + index];
}

unsigned long long Tokenizer::parseHex(const std::string & number)
{
	unsigned long long result = 0;

	for (int i = 0; i <number.size(); ++i)
	{

		if (isNumber(number[i]))
		{
			result *= 16;
			result += number[i] - '0';
		}
		else if (isHexNumber(number[i]))
		{
			char t = number[i] < 'Z' ? 'A' : 'a';
			result *= 16;
			result += number[i] - t + 10;
		}

	}

	return result;
}

unsigned long long Tokenizer::parseOct(const std::string & number)
{
	unsigned long long result = 0;

	for (int i = 0; i < number.size(); ++i)
	{
		if (isNumber(number[i]) && number[i]<'8')
		{
			result *= 8;
			result += number[i] - '0';
		}
		else
		{
			addError(std::string("无效的八进制数字"));
		}
	}

	return result;
}

unsigned long long Tokenizer::parseDec(const std::string & number)
{
	unsigned long long result = 0;

	for (int i = 0; i < number.size(); ++i)
	{
		result *= 10;
		result += number[i] - '0';
	}

	return result;
}

bool Tokenizer::hasMore(int index)
{
	return data->currentIndex + index<data->allToken.size();
}

bool Tokenizer::isPre() const
{
	if (data->allToken.size() == 0)
	{
		return true;
	}

	return data->allToken[data->allToken.size()-2].isBinary();
}

char Tokenizer::getChar()
{
	if (data->codeIndex >= data->code.size())
	{
		++data->codeIndex;
		return -1;
	}

	++data->codeIndex;
	if (data->code[data->codeIndex - 1] == '\n')
	{
		++data->currentLine;
	}

	return data->code[data->codeIndex - 1];
}

unsigned long long Tokenizer::parseTran(unsigned long long temp)
{

	if (temp == 'a')
	{
		return '\a';
	}
	else if (temp == 'b')
	{
		return '\b';
	}
	else if (temp == 'f')
	{
		return '\b';
	}
	else if (temp == 'n')
	{
		return '\n';
	}
	else if (temp == 't')
	{
		return '\t';
	}
	else if (temp == 't')
	{
		return '\v';
	}
	else if (temp == '\\')
	{
		return '\\';
	}
	else if (temp == '\'')
	{
		return '\'';
	}
	else if (temp == '\"')
	{
		return '\"';
	}
	else if (temp == '?')
	{
		return '?';
	}
	else if (temp == '0')
	{
		return 0;
	}
	else if (isNumber(temp)&&temp<'8')
	{
		std::string number;
		number += temp;

		for (int i = 0; i < 2; ++i)
		{
			if (isNumber(peekChar())&&peekChar()<'8')
			{
				number += getChar();
			}
			else
			{
				break;
			}
		}

		return parseOct(number);
	}
	else if (temp == 'x'||temp=='X')
	{
		std::string number;

		for (int i = 0; i < 2; ++i)
		{
			if (isHexNumber(peekChar()))
			{
				number += getChar();
			}
			else
			{
				break;
			}
		}

		return parseHex(number);
	}

	return -1;
}

char Tokenizer::peekChar(int index)
{
	if (data->codeIndex+index >= data->code.size())
	{
		return -1;
	}

	return data->code[data->codeIndex+index];
}
