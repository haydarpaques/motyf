#include <cstdlib>
#include <cstring>
#include <cctype>
#include "lex.hpp"

motyf::lex::lex(const char *str)
{
	std::size_t sz = std::strlen(str);

	this->text = (char *) std::malloc(sz + sizeof(char));
	if (this->text == NULL) {
		this->err = error::allocation_failure;
		return;
	}

	std::memcpy(this->text, str, sz);
	this->text[sz] = '\0';
	this->current = text;
	this->err = error::no_error;
}

motyf::lex::~lex()
{
	if (this->text == NULL)
		return;

	std::free(this->text);
}

bool motyf::lex::is_error() const
{
	return this->err != motyf::error::no_error;
}

motyf::error motyf::lex::get_error() const
{
	return this->err;
}

motyf::token motyf::lex::next()
{
	return this->analyze(true);
}

motyf::token motyf::lex::lookahead()
{
	return this->analyze(false);
}

bool motyf::lex::match(token t)
{
	return t == this->analyze(true);
}

bool motyf::lex::match_lookahead(token t)
{
	return t == this->analyze(false);
}

#define lex_return(t,i) \
do { if (proceed) (this->current) += i; return t; } while(false)
motyf::token motyf::lex::analyze(bool proceed)
{
	while (this->is_ignored(*(this->current)))
		++(this->current);

	// skip inline comments
	while (*(this->current) == '/' && *(this->current + 1) == '/') {
		for (; *(this->current) != '\n'; ++(this->current))
			/* nothing */;
		++(this->current);
		while (this->is_ignored(*(this->current)))
			++(this->current);
	}

	// TODO: skip multiline comments

	this->length    = 0UL;
	this->lexeme[0] = '\0';

	switch (*(this->current))
	{
	case '\0':
		return token::null;
	case '\n':
		lex_return(token::newline, 1);
	case '~':
		lex_return(token::tilde, 1);
	case '`':
		lex_return(token::backtick, 1);
	case '!':
		lex_return(token::exclamation, 1);
	case '@':
		lex_return(token::at, 1);
	case '#':
		lex_return(token::number_sign, 1);
	case '$':
		lex_return(token::dollar, 1);
	case '%':
		lex_return(token::percent, 1);
	case '^':
		lex_return(token::caret, 1);
	case '&':
		lex_return(token::ampersand, 1);
	case '*':
		lex_return(token::asterisk, 1);
	case '(':
		lex_return(token::left_parenthesis, 1);
	case ')':
		lex_return(token::right_parenthesis, 1);
	case '-':
		lex_return(token::minus, 1);
	case '_':
		lex_return(token::underscore, 1);
	case '=':
		lex_return(token::equal_sign, 1);
	case '+':
		lex_return(token::plus, 1);
	case '[':
		lex_return(token::left_bracket, 1);
	case '{':
		lex_return(token::left_brace, 1);
	case ']':
		lex_return(token::right_bracket, 1);
	case '}':
		lex_return(token::right_brace, 1);
	case '\\':
		lex_return(token::backslash, 1);
	case '|':
		lex_return(token::vertical_bar, 1);
	case ';':
		lex_return(token::semicolon, 1);
	case ':':
		lex_return(token::colon, 1);
	case '\'':
		lex_return(token::apostrophe, 1);
	case '"':
		lex_return(token::quotation, 1);
	case ',':
		lex_return(token::comma, 1);
	case '<':
		lex_return(token::left_chevron, 1);
	case '.':
		lex_return(token::dot, 1);
	case '>':
		lex_return(token::right_chevron, 1);
	case '/':
		lex_return(token::slash, 1);
	case '?':
		lex_return(token::question_mark, 1);

	default:
		// TODO: check buffer size to prevent buffer overrun
		if (is_key_or_id(*(this->current))) {
			while (is_legal(*(this->current + this->length))) {
				this->lexeme[this->length] = *(this->current + this->length);
				++(this->length);
			}

			this->lexeme[this->length] = '\0';
			lex_return(token::id, this->length);
		} else {
			while (is_numeric(*(this->current + this->length))) {
				this->lexeme[this->length] = *(this->current + this->length);
				++(this->length);
			}

			this->lexeme[this->length] = '\0';
			lex_return(token::numeric, this->length);
		}

		lex_return(token::unknown, 1);
	}
}

bool motyf::lex::is_ignored(char c)
{
	if ((c > 0x00 && c < 0x0a) || (c > 0x0a && c < 0x21) || c == 0x7f)
		return true;

	return false;
}

bool motyf::lex::is_legal(char c)
{
	if (std::isalnum(c) || c == 0x2d || c == 0x5f)
		return true;

	return false;
}

bool motyf::lex::is_key_or_id(char c)
{
	if (std::isalpha(c) || c == 0x5f)
		return true;

	return false;
}

bool motyf::lex::is_numeric(char c)
{
	return (bool) std::isdigit(c);
}

const char *motyf::lex::get_lexeme() const
{
	return this->lexeme;
}
