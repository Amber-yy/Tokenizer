#pragma once

#include "Token.h"
#include "Judge.h"

class Tokenizer
{
public:
	Tokenizer();
	~Tokenizer();
	void addError(std::string &info);
	void parse(std::string &code);
	const Token &read();
	const Token &peek(int index);
protected:
	unsigned long long parseHex(const std::string &number);
	unsigned long long parseOct(const std::string &number);
	unsigned long long parseDec(const std::string &number);
	void getComments();
	void getNumber();
	void getId();
	void getStringLiteral();
	void getSub();
	void getAdd();
	void getMulti();
	void getDivi();
	void getMod();
	void getAssign();
	void getMember();
	void getComma();
	void getNegation();
	void getGreater();
	void getLess();
	void getAnd();
	void getOr();
	void getLeftBracket();
	void getRightBracket();
	void getLeftSqBracket();
	void getRightSqBracket();
	void getLeftCurBracket();
	void getRightCurBracket();
	void getConditionLeft();
	void getConditionRight();
	void getEnter();
	void getEndOfState();
	void getBitXor();
	void getBitNegation();
	void unGetChar();
	char getChar();
	unsigned long long parseTran(unsigned long long c);
	char peekChar(int index = 0);
	bool hasMore(int index = 0);
	bool isPre()const;
protected:
	struct Data;
	Data *data;
};

