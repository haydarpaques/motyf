#ifndef MOTYF_LEX_HPP
#define MOTYF_LEX_HPP

#include <cstddef>
#include "error.hpp"

#define MOTYF_LEX_MAXBUF (128)

namespace Motyf {
	enum class Token {
		Null, // Null character '\0'
		Unknown, // Unrecognize character
		Newline, // Newline/linefeed character '\n'
		Tilde, // ~
		Backtick, // `
		Exclamation, // !
		At, // @
		NumberSign, // #
		Dollar, // $
		Percent, // %
		Caret, // ^
		Ampersand, // &
		Asterisk, // *
		LeftParenthesis, // (
		RightParenthesis, // )
		Minus, // -
		Underscore, // _
		EqualSign, // =
		Plus, // +
		LeftBracket, // [
		LeftBrace, // {
		RightBracket, // ]
		RightBrace, // }
		Backslash, //
		VerticalBar, // |
		Semicolon, // ;
		Colon, // :
		Apostrophe, // '
		Quotation, // "
		Comma, // ,
		LeftChevron, // <
		Dot, // .
		RightChevron, // >
		Slash, // /
		QuestionMark, // ?
		Numeric, // Numeric literal / constant
		ID, // Identifier, keyword, or naked string literal
	};

	class Lex {
		char        *text;
		const char  *current;
		std::size_t  length;
		Error        error;
		char         lexeme[MOTYF_LEX_MAXBUF];

		/**
		 * @brief
		 * Does the actual work around here.
		 */
		Token lex(bool);

		/**
		 * @brief
		 * Returns true if provided char is to be ignored by
		 * this lexical analyzer.
		 */
		bool isIgnored(char);

		/**
		 * @brief
		 * Returns true if provided char is legal for identifiers.
		 */
		bool isLegal(char);

		/**
		 * @brief
		 * Returns true if provided char marks the beginning of a keyword
		 * an identifier.
		 */
		bool isKeyOrIdentifier(char);

		/**
		 * @brief
		 * Returns true if provided char marks the beginning of a numeric
		 * literal / constant.
		 */
		bool isNumeric(char);

	public:
		Lex(const char *);

		~Lex();

		/**
		 * @brief
		 * Checks whether the lexical analyzer is in error state.
		 */
		bool isError() const;

		/**
		 * @brief
		 * Returns last error occured.
		 */
		Error getError() const;

		/**
		 * @brief
		 * Returns next token and progresses the lexical analyzer.
		 */
		Token next();

		/**
		 * @brief
		 * Returns next token and without progressing the lexical analyzer.
		 */
		Token lookahead();

		/**
		 * @brief
		 * Expects next token to match the given token.
		 */
		bool match(Token);

		/**
		 * @brief
		 * Expects next token to match the given token without progressing
		 * the lexical analyzer.
		 */
		bool matchLookahead(Token);

		/**
		 * @brief
		 * Returns the lexeme of last recognized token.
		 * Note that this lexical anayzer shall not store lexeme
		 * of singulative tokens.
		 */
		const char *getLexeme() const;
	};
}

#endif
