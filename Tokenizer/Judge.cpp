#include "Judge.h"

bool isSpace(char c)
{
	return 0 <= c&&c <= ' ';
}

bool isNumber(char c)
{
	return '0' <= c &&c <= '9';
}

bool isLetter(char c)
{
	return ('A' <= c&&c <= 'Z') || ('a' <= c&&c <= 'z');
}

bool isIdStart(char c)
{
	return isLetter(c) || c == '-';
}

bool isHexNumber(char c)
{
	return ('a' <= c&&c <= 'f') || ('A' <= c&&c <= 'F');
}

bool isReal(char c, bool hex)
{
	return isNumber(c) || c == '.'||(hex&&isHexNumber(c));
}

bool isId(char c)
{
	return isIdStart(c) || isNumber(c);
}

bool isPrimary(char c)
{
	return isId(c) || c == '\'' || c == '\"';
}

int isQuotes(char c)
{
	if (c == '\'')
	{
		return 1;
	}
	else if (c == '\"')
	{
		return 2;
	}
	return 0;
}

bool isDiv(char c)
{
	return !isId(c);
}

bool isKeyword(std::string &w)
{
	if (w == "auto" ||w=="bool" ||w == "break" || w == "case" || w == "char" ||
		w == "const" || w == "contine" || w == "default" || w == "do" || w == "double" ||
		w == "else" || w == "enum" || w == "extern" || w == "float" 
		|| w == "for" || w == "goto" || w == "if" ||w== "inline" || w == "int"
		|| w == "long" || w == "register" || w == "return" || w == "short"
		|| w == "signed" || w == "sizeof" || w == "static" || w == "struct"
		|| w == "switch" || w == "typedef" || w == "union" || w == "unsigned"
		|| w == "void" || w == "volatile" || w == "while"||w=="wchar_t")
	{
		return true;
	}

	return false;
}
