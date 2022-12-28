#include <iostream>
#include <cstring>
#include "parser.hpp"

#define proceed_parser(fn) \
if (! fn) return false

#define return_syntax_error(err) \
do { std::cerr << "syntax error: " << err << '\n'; return false; } while (false)

motyf::parser::parser(lex *le)
{
	this->lexer = le;
}

bool motyf::parser::module()
{
	if (! this->lexer->match(token::id))
		return_syntax_error("expect a module name declaration");

	if (strcmp("module", this->lexer->get_lexeme()) != 0)
		return_syntax_error("expect a module name declaration");

	if (! this->lexer->match(token::id))
		return_syntax_error("module what?");

	while (this->lexer->lookahead() == token::dot) {
		(void) this->lexer->next();
		if (! this->lexer->match(token::id))
			return_syntax_error("malformed module declaration");
	}

	if (! this->lexer->match(token::newline))
		return_syntax_error("expect a newline after module declaration");

	return true;
}

bool motyf::parser::function()
{
	proceed_parser(this->function_prototype());

	if (! this->lexer->match(token::left_brace))
		return_syntax_error("expect opening brace '{'");

	proceed_parser(this->function_body());

	if (! this->lexer->match(token::right_brace))
		return_syntax_error("expect closing brace '}'");

	return true;
}

bool motyf::parser::function_prototype()
{
	if (! this->lexer->match(token::id))
		return_syntax_error("expect function definition keyword 'func'");

	if (strcmp("func", this->lexer->get_lexeme()) != 0)
		return_syntax_error("expect 'func'");

	if (! this->lexer->match(token::id))
		return_syntax_error("expect function identifier");

	if (! this->lexer->match(token::colon))
		return_syntax_error("expect a colon ':' after function identifier");

	if (! this->lexer->match(token::left_parenthesis))
		return_syntax_error("expect opening parenthesis '('");

	if (! this->lexer->match(token::right_parenthesis))
		return_syntax_error("expect closing parenthesis ')'");

	if (this->lexer->match_lookahead(token::minus))
		proceed_parser(this->function_return());

	return true;
}

bool motyf::parser::function_body()
{
	// TODO
	return true;
}

bool motyf::parser::function_return()
{
	(void) this->lexer->match(token::minus);

	if (! this->lexer->match(token::right_chevron))
		return_syntax_error("expect arrow before return data types '->'");

	proceed_parser(this->datatype_list());

	return true;
}

bool motyf::parser::datatype_list()
{
	if (! this->lexer->match(token::id))
		return_syntax_error("expect list of return datatype");

	if (this->lexer->match_lookahead(token::comma)) {
		(void) this->lexer->match(token::comma);

		proceed_parser(this->datatype_list());
	}

	return true;
}
