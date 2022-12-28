#include <iostream>
#include <string>
#include "lex.hpp"

extern const char *token_string[];
int test_lex(const char *);

int main(void)
{
	std::string in;
	std::getline(std::cin, in, '\0');

	if (test_lex(in.c_str()) == -1)
		exit(EXIT_FAILURE);

	exit(EXIT_SUCCESS);
}

int test_lex(const char *in)
{
	motyf::lex   le(in);
	motyf::token to;

	if (le.is_error())
		return -1;

	while ((to = le.next()) != motyf::token::null) {
		std::cout << "Get: " << token_string[(int) to] << '\n';

		if (to == motyf::token::id || to == motyf::token::numeric)
			std::cout << "\tLexeme: \"" << le.get_lexeme() << "\"\n";
	}

	return 0;
}

const char *token_string[] = {
	"Null", "Unknown", "Newline", "Tilde", "Backtick", "Exclamation", "At", "NumberSign",
	"Dollar", "Percent", "Caret", "Ampersand", "Asterisk", "LeftParenthesis",
	"RightParenthesis", "Minus", "Underscore", "EqualSign", "Plus", "LeftBracket",
	"LeftBrace", "RightBracket", "RightBrace", "Backslash", "VerticalBar", "Semicolon",
	"Colon", "Apostrophe", "Quotation", "Comma", "LeftChevron", "Dot", "RightChevron",
	"Slash", "QuestionMark", "Numeric", "ID"
};
