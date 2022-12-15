#include <iostream>
#include <string>
#include "lex.hpp"

extern const char *tokenString[];
int testLex(const char *);

int main(void)
{
	std::string in;
	std::getline(std::cin, in, '\0');

	if (testLex(in.c_str()) == -1)
		exit(EXIT_FAILURE);

	exit(EXIT_SUCCESS);
}

int testLex(const char *in)
{
	Motyf::Lex   lex(in);
	Motyf::Token token;

	if (lex.isError())
		return -1;

	while ((token = lex.next()) != Motyf::Token::Null) {
		std::cout << "Get: " << tokenString[(int) token] << '\n';

		if (token == Motyf::Token::ID || token == Motyf::Token::Numeric)
			std::cout << "\tLexeme: \"" << lex.getLexeme() << "\"\n";
	}

	return 0;
}

const char *tokenString[] = {
	"Null", "Unknown", "Newline", "Tilde", "Backtick", "Exclamation", "At", "NumberSign",
	"Dollar", "Percent", "Caret", "Ampersand", "Asterisk", "LeftParenthesis",
	"RightParenthesis", "Minus", "Underscore", "EqualSign", "Plus", "LeftBracket",
	"LeftBrace", "RightBracket", "RightBrace", "Backslash", "VerticalBar", "Semicolon",
	"Colon", "Apostrophe", "Quotation", "Comma", "LeftChevron", "Dot", "RightChevron",
	"Slash", "QuestionMark", "Numeric", "ID"
};
