#include "Token.h"

#include <string>

struct Token::Data
{
	std::string origin;
	OperatorType opType;
	KeyWordType keyType;
	double vr;
	unsigned long long vi;
	int tokenType;
	int line;
};

Token::Token(Token && t)
{
	*this = std::move(t);
}

Token::Token(const Token & t)
{
	data = new Data;
	*this = t;
}

Token::Token()
{
	data = new Data;
	data->tokenType = NoType;
}

Token::~Token()
{
	delete data;
}

Token & Token::operator=(const Token & t)
{
	data->keyType = t.data->keyType;
	data->opType = t.data->opType;
	data->tokenType = t.data->tokenType;
	return *this;
}

Token & Token::operator=(Token && t)
{
	data = t.data;
	t.data = nullptr;
	return *this;
}

void Token::setKeyWord(KeyWordType type)
{
	data->keyType = type;
}

void Token::setOperator(OperatorType type)
{
	data->opType = type;
}

void Token::setLine(int line)
{
	data->line = line;
}

void Token::setString(std::string & str)
{
	data->origin = std::move(str);
}

void Token::setReal(double n)
{
	data->vr = n;
}

void Token::setInteger(unsigned long long n)
{
	data->vi = n;
}

void Token::addType(int type)
{
	data->tokenType |= type;
}

void Token::clearType()
{
	data->tokenType = NoType;
}

bool Token::isIntLiteral() const
{
	return (data->tokenType&IntLiteral)==IntLiteral;
}

bool Token::isRealLiteral() const
{
	return (data->tokenType&RealLiteral) == RealLiteral;
}

bool Token::isStringLiteral() const
{
	return (data->tokenType&StringLiteral) == StringLiteral;
}

bool Token::isIdentifier() const
{
	return (data->tokenType&Identifier) == Identifier;
}

bool Token::isKeyWord() const
{
	return (data->tokenType&KeyWord )== KeyWord;
}

bool Token::isOperator() const
{
	return (data->tokenType&Operator) == Operator;
}

bool Token::isEol() const
{
	return (data->tokenType&EndOfLine) == EndOfLine;
}

bool Token::isEos() const
{
	return (data->tokenType&EndOfState) == EndOfState;
}

bool Token::isEof() const
{
	return (data->tokenType&EndOfFile) == EndOfFile;
}

bool Token::isNormalId() const
{
	return isIdentifier() && !isKeyWord();
}

bool Token::isNormalValue() const
{
	return isLiteral()||isNormalId();
}

bool Token::isTypeName() const
{
	return isKeyWord()&&data->keyType>=Bool&&data->keyType<=Double;
}

bool Token::isValue() const
{
	return isLiteral()||(isIdentifier()&&!isKeyWord());
}

bool Token::isLiteral() const
{
	return isIntLiteral()||isRealLiteral()||isStringLiteral();
}

bool Token::isBinary() const
{
	return isOperator()&&((Divi <=data->opType&&data->opType<=Or)||(Assign <= data->opType&&data->opType <= Comma));
}

OperatorType Token::getOperator() const
{
	return data->opType;
}

unsigned long long Token::getInteger() const
{
	return data->vi;
}

const std::string & Token::getString() const
{
	return data->origin;
}
