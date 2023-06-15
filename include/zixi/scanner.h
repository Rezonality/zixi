#pragma once

namespace Zixi
{
typedef enum
{
    // Single-character tokens.
    TOKEN_LEFT_PAREN,
    TOKEN_RIGHT_PAREN,
    TOKEN_LEFT_BRACE,
    TOKEN_RIGHT_BRACE,
    TOKEN_COMMA,
    TOKEN_DOT,
    TOKEN_MINUS,
    TOKEN_PLUS,
    TOKEN_SEMICOLON,
    TOKEN_SLASH,
    TOKEN_STAR,
    // One or two character tokens.
    TOKEN_BANG,
    TOKEN_BANG_EQUAL,
    TOKEN_EQUAL,
    TOKEN_EQUAL_EQUAL,
    TOKEN_GREATER,
    TOKEN_GREATER_EQUAL,
    TOKEN_LESS,
    TOKEN_LESS_EQUAL,
    // Literals.
    TOKEN_IDENTIFIER,
    TOKEN_STRING,
    TOKEN_NUMBER,
    // Keywords.
    TOKEN_ERROR,
    TOKEN_TUNING,
    TOKEN_TEMPO,
    TOKEN_FUTURE,
    TOKEN_SWAP,
    TOKEN_SHAKE,
    TOKEN_DOZE,
    TOKEN_PERK,
    TOKEN_PERCUSSIVE_PATTERN,
    TOKEN_MELODIC_PATTERN,
    TOKEN_CONCRETE_PATTERN,
    TOKEN_PRODUCES,
    TOKEN_ACTION_SHIFT,
    TOKEN_ACTION_GREATER_GREATER,
    TOKEN_ACTION_LESS_LESS,
    TOKEN_HAT,
    TOKEN_COLON,
    TOKEN_DOLLAR,
    TOKEN_EOF
} TokenType;

typedef struct
{
    TokenType type;
    const char* start;
    int length;
    int line;
} Token;

typedef struct
{
    const char* start;
    const char* current;
    int line;
} Scanner;

void zixi_scanner_init(Scanner& scanner, const char* pszSource);
Token zixi_scanner_next_token(Scanner& scanner);
const char* zixi_token_to_string(TokenType& token);

} //namespace Zixi
