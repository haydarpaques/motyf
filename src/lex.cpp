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

#define lex_return(t,i) \
do { if (proceed) (this->current) += i; return t; } while(false)
Motyf::Token Motyf::Lex::lex(bool proceed)
{
	while (this->isIgnored(*(this->current)))
		++(this->current);

	// skip inline comments
	while (*(this->current) == '/' && *(this->current + 1) == '/') {
		for (; *(this->current) != '\n'; ++(this->current))
			/* nothing */;
		++(this->current);
		while (this->isIgnored(*(this->current)))
			++(this->current);
	}

	// TODO: skip multiline comments

	this->length    = 0UL;
	this->lexeme[0] = '\0';

	switch (*(this->current))
	{
	case '\0':
		return Token::Null;
	case '\n':
		lex_return(Token::Newline, 1);
	case '~':
		lex_return(Token::Tilde, 1);
	case '`':
		lex_return(Token::Backtick, 1);
	case '!':
		lex_return(Token::Exclamation, 1);
	case '@':
		lex_return(Token::At, 1);
	case '#':
		lex_return(Token::NumberSign, 1);
	case '$':
		lex_return(Token::Dollar, 1);
	case '%':
		lex_return(Token::Percent, 1);
	case '^':
		lex_return(Token::Caret, 1);
	case '&':
		lex_return(Token::Ampersand, 1);
	case '*':
		lex_return(Token::Asterisk, 1);
	case '(':
		lex_return(Token::LeftParenthesis, 1);
	case ')':
		lex_return(Token::RightParenthesis, 1);
	case '-':
		lex_return(Token::Minus, 1);
	case '_':
		lex_return(Token::Underscore, 1);
	case '=':
		lex_return(Token::EqualSign, 1);
	case '+':
		lex_return(Token::Plus, 1);
	case '[':
		lex_return(Token::LeftBracket, 1);
	case '{':
		lex_return(Token::LeftBrace, 1);
	case ']':
		lex_return(Token::RightBracket, 1);
	case '}':
		lex_return(Token::RightBrace, 1);
	case '\\':
		lex_return(Token::Backslash, 1);
	case '|':
		lex_return(Token::VerticalBar, 1);
	case ';':
		lex_return(Token::Semicolon, 1);
	case ':':
		lex_return(Token::Colon, 1);
	case '\'':
		lex_return(Token::Apostrophe, 1);
	case '"':
		lex_return(Token::Quotation, 1);
	case ',':
		lex_return(Token::Comma, 1);
	case '<':
		lex_return(Token::LeftChevron, 1);
	case '.':
		lex_return(Token::Dot, 1);
	case '>':
		lex_return(Token::RightChevron, 1);
	case '/':
		lex_return(Token::Slash, 1);
	case '?':
		lex_return(Token::QuestionMark, 1);

	default:
		// TODO: check buffer size to prevent buffer overrun
		if (isKeyOrIdentifier(*(this->current))) {
			while (isLegal(*(this->current + this->length))) {
				this->lexeme[this->length] = *(this->current + this->length);
				++(this->length);
			}

			this->lexeme[this->length] = '\0';
			lex_return(Token::ID, this->length);
		} else {
			while (isNumeric(*(this->current + this->length))) {
				this->lexeme[this->length] = *(this->current + this->length);
				++(this->length);
			}

			this->lexeme[this->length] = '\0';
			lex_return(Token::Numeric, this->length);
		}

		lex_return(Token::Unknown, 1);
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
