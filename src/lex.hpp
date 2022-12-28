#ifndef MOTYF_LEX_HPP
#define MOTYF_LEX_HPP

#include <cstddef>
#include "error.hpp"

#define MOTYF_LEX_MAXBUF (128)

namespace motyf {
	enum class token {
		null, // Null character '\0'
		unknown, // Unrecognize character
		newline, // Newline/linefeed character '\n'
		tilde, // ~
		backtick, // `
		exclamation, // !
		at, // @
		number_sign, // #
		dollar, // $
		percent, // %
		caret, // ^
		ampersand, // &
		asterisk, // *
		left_parenthesis, // (
		right_parenthesis, // )
		minus, // -
		underscore, // _
		equal_sign, // =
		plus, // +
		left_bracket, // [
		left_brace, // {
		right_bracket, // ]
		right_brace, // }
		backslash, //
		vertical_bar, // |
		semicolon, // ;
		colon, // :
		apostrophe, // '
		quotation, // "
		comma, // ,
		left_chevron, // <
		dot, // .
		right_chevron, // >
		slash, // /
		question_mark, // ?
		numeric, // Numeric literal / constant
		id, // Identifier, keyword, or naked string literal
	};

	class lex {
		char        *text;
		const char  *current;
		std::size_t  length;
		error        err;
		char         lexeme[MOTYF_LEX_MAXBUF];

		/**
		 * @brief
		 * Does the actual work around here.
		 */
		token analyze(bool);

		/**
		 * @brief
		 * Returns true if provided char is to be ignored by
		 * this lexical analyzer.
		 */
		bool is_ignored(char);

		/**
		 * @brief
		 * Returns true if provided char is legal for identifiers.
		 */
		bool is_legal(char);

		/**
		 * @brief
		 * Returns true if provided char marks the beginning of a keyword
		 * or an identifier.
		 */
		bool is_key_or_id(char);

		/**
		 * @brief
		 * Returns true if provided char marks the beginning of a numeric
		 * literal / constant.
		 */
		bool is_numeric(char);

	public:
		lex(const char *);

		~lex();

		/**
		 * @brief
		 * Checks whether the lexical analyzer is in error state.
		 */
		bool is_error() const;

		/**
		 * @brief
		 * Returns last error occured.
		 */
		error get_error() const;

		/**
		 * @brief
		 * Returns next token and progresses the lexical analyzer.
		 */
		token next();

		/**
		 * @brief
		 * Returns next token and without progressing the lexical analyzer.
		 */
		token lookahead();

		/**
		 * @brief
		 * Expects next token to match the given token.
		 */
		bool match(token);

		/**
		 * @brief
		 * Expects next token to match the given token without progressing
		 * the lexical analyzer.
		 */
		bool match_lookahead(token);

		/**
		 * @brief
		 * Returns the lexeme of last recognized token.
		 * Note that this lexical anayzer shall not store lexeme
		 * of singulative tokens.
		 */
		const char *get_lexeme() const;
	};
}

#endif
