#pragma once

#include "natalie_parser/lexer/interpolated_string_lexer.hpp"
#include "natalie_parser/token.hpp"
#include "tm/shared_ptr.hpp"
#include "tm/vector.hpp"

namespace NatalieParser {

class Lexer {
public:
    Lexer(SharedPtr<String> input, SharedPtr<String> file)
        : m_input { input }
        , m_file { file }
        , m_size { input->length() } { }

    SharedPtr<Vector<Token>> tokens();
    Token next_token();

private:
    char current_char() {
        if (m_index >= m_size)
            return 0;
        return m_input->at(m_index);
    }

    bool match(size_t bytes, const char *compare);
    void advance();
    void advance(size_t bytes);
    void rewind(size_t bytes = 1);

    char next() {
        advance();
        return current_char();
    }

    char peek() {
        if (m_index + 1 >= m_size)
            return 0;
        return m_input->at(m_index + 1);
    }

    bool skip_whitespace();
    Token build_next_token();
    Token consume_symbol();
    Token consume_word(Token::Type type);
    Token consume_bare_name();
    Token consume_constant();
    Token consume_global_variable();
    Token consume_heredoc();
    Token consume_numeric();
    long long consume_hex_number(int max_length = 0, bool allow_underscore = false);
    long long consume_octal_number(int max_length = 0, bool allow_underscore = false);
    Token consume_double_quoted_string(char delimiter);
    Token consume_single_quoted_string(char delimiter);
    Token consume_quoted_array_without_interpolation(char delimiter, Token::Type type);
    Token consume_quoted_array_with_interpolation(char delimiter, Token::Type type);
    Token consume_regexp(char delimiter);
    SharedPtr<String> consume_non_whitespace();
    void utf32_codepoint_to_utf8(String &buf, long long codepoint);

    SharedPtr<String> m_input;
    SharedPtr<String> m_file;
    size_t m_size { 0 };
    size_t m_index { 0 };

    // if non-zero, the index we should continue on after a linebreak
    size_t m_index_after_heredoc { 0 };

    // current character position
    size_t m_cursor_line { 0 };
    size_t m_cursor_column { 0 };

    // start of current token
    size_t m_token_line { 0 };
    size_t m_token_column { 0 };

    // if the current token is preceded by whitespace
    bool m_whitespace_precedes { false };

    // the previously-matched token
    Token m_last_token {};
};
}
