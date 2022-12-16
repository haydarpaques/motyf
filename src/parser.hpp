#ifndef MOTYF_PARSER_HPP
#define MOTYF_PARSER_HPP

#include "lex.hpp"

namespace motyf {
	/**
	 * @brief
	 * Motyf's recursive descent parser. This whole part of the compiler is not yet
	 * 'flattened' for optimization.
	 * TODO: flatten the parser for optimization.
	 * TODO: integrate parser with code generator.
	 * TODO: better way to initiate lexer?
	 */
	class parser {
		lex *lexer;

		/**
		 * @brief
		 * Checks whether the given string is a reserved keyword.
		 */
		bool is_keyword(const char *);

		/**
		 * @brief
		 * Expects given string to match some keyword.
		 */
		bool match_keyword(const char *);
	public:
		parser(lex *);

		bool module();
		bool function();
		bool function_prototype();
		bool function_body();
		bool function_return();
		bool datatype_list();
	};
}

#endif
