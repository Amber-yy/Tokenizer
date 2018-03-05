#pragma once

#include <string>

const int	NoType = 0,
IntLiteral = 1,
RealLiteral = 1 << 1,
StringLiteral = 1 << 2,
Identifier = 1 << 3,
KeyWord = 1 << 4,
Operator = 1 << 5,
EndOfLine = 1 << 6,
EndOfState = 1 << 7,
EndOfFile = 1 << 8;

enum KeyWordType
{
	Bool=0,//���͡��ؼ��֡�������
	Char=1,
	WChar=2,
	Short=3,
	Long=4,
	Int=5,
	Float=6,
	Double=7,
	Auto=8,
	Break=9,
	Case=10,
	Const=11,
	Continue=12,
	Default=13,
	Do=14,
	Else=15,
	Enum=16,
	Extern=17,
	For=18,
	Goto=19,
	If=20,
	Register=21,
	Return=22,
	Signed=23,
	Sizeof=24,
	Static=25,
	Struct=26,
	Switch=27,
	Typedef=28,
	Union=29,
	Unsigned=30,
	Void=31,
	Volatile=32,
	While=33,
};

enum OperatorType
{
	LeftBracket=0,	//3������
	RightBracket=1,
	LeftSqBracket=2,
	RightSqBracket=3,
	LeftCurBarack=4,
	RightCurBarack=5,

	Member=6,//��1���ȼ������������
	MemberP=7,

	Negative=8,//��2���ȼ���������ҵ���
	Positive=9,
	TypeTran=10,
	PostIncrement=11,
	PostDecrement=12,
	PreIncrement=13,
	PreDecrement=14,
	GetValue=15,
	GetAddr=16,
	Negation=17,
	BitNegation=18,
	SizeofOp=19,

	Divi=20,//��3���ȼ�������
	Multi=21,
	Mod=22,

	Add=23,//��4���ȼ�������
	Sub=24,

	LeftMove=25,//��5���ȼ�������
	RightMove=26,

	Greater=27,//��6���ȼ�������
	GreaterEqual=28,
	Less=29,
	LessEqual=30,

	Equal=31,//��7���ȼ�������
	NotEqual=32,

	BitAnd=33,//��8���ȼ�������
	BitXor=34,//��9���ȼ�������
	BitOr=35,//��10���ȼ�������

	And=36,//��11���ȼ�������
	Or=37,//��12���ȼ�������

	ConditionLeft=38,//��13���ȼ����ҵ���
	ConditionRight=39,

	Assign=40,//��14���ȼ����ҵ���
	DiviAssign=41,
	MultiAssign=42,
	ModAssign=43,
	AddAssign=44,
	SubAssign=45,
	LeftMoveAssign=46,
	RightMoveAssign=47,
	BitAndAssign=48,
	BitXorAssign=49,
	BitOrAssign=50,

	Comma=51,//��15���ȼ����ҵ���
};

class Token
{
public:
	Token();
	Token(Token &&t);
	Token(const Token &t);
	~Token();
	Token &operator=(const Token&t);
	Token &operator=(Token &&t);
	void setKeyWord(KeyWordType type);
	void setOperator(OperatorType type);
	void setLine(int line);
	void setString(std::string &str);
	void setReal(double n);
	void setInteger(unsigned long long n);
	void addType(int type);
	void clearType();
	bool isIntLiteral()const;
	bool isRealLiteral()const;
	bool isStringLiteral()const;
	bool isIdentifier()const;
	bool isKeyWord()const;
	bool isOperator()const;
	bool isEol()const;
	bool isEos()const;
	bool isEof()const;
	bool isNormalId()const;
	bool isNormalValue() const;
	bool isTypeName()const;
	bool isValue()const;
	bool isLiteral()const;
	bool isBinary()const;
	OperatorType getOperator() const;
	unsigned long long getInteger()const;
	const std::string &getString()const;
protected:
	struct Data;
	Data *data;
};

