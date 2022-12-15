#include <cstdlib>
#include <cstring>
#include <cctype>
#include "lex.hpp"

Motyf::Lex::Lex(const char *str)
{
	std::size_t sz = std::strlen(str);

	this->text = (char *) std::malloc(sz + sizeof(char));
	if (this->text == NULL) {
		this->error = Error::AllocationFailure;
		return;
	}

	std::memcpy(this->text, str, sz);
	this->text[sz] = '\0';
	this->current = text;
	this->error = Error::NoError;
}

Motyf::Lex::~Lex()
{
	if (this->text == NULL)
		return;

	std::free(this->text);
}

bool Motyf::Lex::isError() const
{
	return this->error != Motyf::Error::NoError;
}

Motyf::Error Motyf::Lex::getError() const
{
	return this->error;
}

Motyf::Token Motyf::Lex::next()
{
	return this->lex(true);
}

Motyf::Token Motyf::Lex::lookahead()
{
	return this->lex(false);
}

bool Motyf::Lex::match(Token t)
{
	return t == this->lex(true);
}

bool Motyf::Lex::matchLookahead(Token t)
{
	return t == this->lex(false);
}

#define lex_return(t) \
do { if (proceed) ++(this->current); return t; } while(false)
Motyf::Token Motyf::Lex::lex(bool proceed)
{
	while (this->isIgnored(*(this->current)))
		++(this->current);

	// ignore comments
	if (*(this->current) == '/') {
		// inline comments
		if (*(this->current + 1) == '/') {
			for (;*(this->current) != '\n'; ++(this->current));

			++(this->current);
			while (this->isIgnored(*(this->current)))
				++(this->current);
		}

		// multiline comments
		if (*(this->current + 1) == '*') {
			// TODO
		}
	}

	this->length    = 0UL;
	this->lexeme[0] = '\0';

	switch (*(this->current))
	{
	case '\0':
		return Token::Null;
	case '\n':
		lex_return(Token::Newline);
	case '~':
		lex_return(Token::Tilde);
	case '`':
		lex_return(Token::Backtick);
	case '!':
		lex_return(Token::Exclamation);
	case '@':
		lex_return(Token::At);
	case '#':
		lex_return(Token::NumberSign);
	case '$':
		lex_return(Token::Dollar);
	case '%':
		lex_return(Token::Percent);
	case '^':
		lex_return(Token::Caret);
	case '&':
		lex_return(Token::Ampersand);
	case '*':
		lex_return(Token::Asterisk);
	case '(':
		lex_return(Token::LeftParenthesis);
	case ')':
		lex_return(Token::RightParenthesis);
	case '-':
		lex_return(Token::Minus);
	case '_':
		lex_return(Token::Underscore);
	case '=':
		lex_return(Token::EqualSign);
	case '+':
		lex_return(Token::Plus);
	case '[':
		lex_return(Token::LeftBracket);
	case '{':
		lex_return(Token::LeftBrace);
	case ']':
		lex_return(Token::RightBracket);
	case '}':
		lex_return(Token::RightBrace);
	case '\\':
		lex_return(Token::Backslash);
	case '|':
		lex_return(Token::VerticalBar);
	case ';':
		lex_return(Token::Semicolon);
	case ':':
		lex_return(Token::Colon);
	case '\'':
		lex_return(Token::Apostrophe);
	case '"':
		lex_return(Token::Quotation);
	case ',':
		lex_return(Token::Comma);
	case '<':
		lex_return(Token::LeftChevron);
	case '.':
		lex_return(Token::Dot);
	case '>':
		lex_return(Token::RightChevron);
	case '/':
		lex_return(Token::Slash);
	case '?':
		lex_return(Token::QuestionMark);

	default:
		// TODO: check buffer size to prevent buffer overrun
		if (isKeyOrIdentifier(*(this->current))) {
			while (isLegal(*(this->current + this->length))) {
				this->lexeme[this->length] = *(this->current + this->length);
				++(this->length);
			}

			this->lexeme[this->length] = '\0';

			if (proceed)
				this->current += this->length;

			return Token::ID;
		} else if (isNumeric(*(this->current))) {
			while (isNumeric(*(this->current + this->length))) {
				this->lexeme[this->length] = *(this->current + this->length);
				++(this->length);
			}

			this->lexeme[this->length] = '\0';

			if (proceed)
				this->current += this->length;

			return Token::Numeric;
		}

		lex_return(Token::Unknown);
	}
}

bool Motyf::Lex::isIgnored(char c)
{
	if ((c > 0x00 && c < 0x0a) || (c > 0x0a && c < 0x21) || c == 0x7f)
		return true;

	return false;
}

bool Motyf::Lex::isLegal(char c)
{
	if (std::isalnum(c) || c == 0x2d || c == 0x5f)
		return true;

	return false;
}

bool Motyf::Lex::isKeyOrIdentifier(char c)
{
	if (std::isalpha(c) || c == 0x5f)
		return true;

	return false;
}

bool Motyf::Lex::isNumeric(char c)
{
	return (bool) std::isdigit(c);
}

const char *Motyf::Lex::getLexeme() const
{
	return this->lexeme;
}
