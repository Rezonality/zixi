#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <unordered_map>

#include <zixi/scanner.h>

namespace Zixi
{

namespace
{
bool isAlpha(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool isDigit(char c)
{
    return c >= '0' && c <= '9';
}

bool isAtEnd(Scanner& scanner)
{
    return *scanner.current == '\0';
}

char advance(Scanner& scanner)
{
    scanner.current++;
    return scanner.current[-1];
}

char peek(Scanner& scanner)
{
    return *scanner.current;
}

char peekNext(Scanner& scanner)
{
    if (isAtEnd(scanner))
        return '\0';
    return scanner.current[1];
}

bool match(Scanner& scanner, char expected)
{
    if (isAtEnd(scanner))
        return false;
    if (*scanner.current != expected)
        return false;
    scanner.current++;
    return true;
}

Token makeToken(Scanner& scanner, TokenType type)
{
    Token token;
    token.type = type;
    token.start = scanner.start;
    token.length = (int)(scanner.current - scanner.start);
    token.line = scanner.line;
    return token;
}

Token errorToken(Scanner& scanner, const char* message)
{
    Token token;
    token.type = TOKEN_ERROR;
    token.start = message;
    token.length = (int)strlen(message);
    token.line = scanner.line;
    return token;
}

void skipWhitespace(Scanner& scanner)
{
    for (;;)
    {
        char c = peek(scanner);
        switch (c)
        {
            case ' ':
            case '\r':
            case '\t':
                advance(scanner);
                break;
            case '\n':
                scanner.line++;
                advance(scanner);
                break;
            case '/':
                if (peekNext(scanner) == '/')
                {
                    // A comment goes until the end of the line.
                    while (peek(scanner) != '\n' && !isAtEnd(scanner))
                        advance(scanner);
                }
                else
                {
                    return;
                }
                break;
            default:
                return;
        }
    }
}

std::unordered_map<std::string, TokenType> identifiers {
    {"tuning", TOKEN_TUNING},
    {"doze", TOKEN_DOZE},
    {"perk", TOKEN_PERK},
    {"swap", TOKEN_SWAP},
    {"shake", TOKEN_SHAKE},
    {"future", TOKEN_FUTURE},
    {"tempo", TOKEN_TEMPO},
    {"shift", TOKEN_ACTION_SHIFT},
    {">>", TOKEN_ACTION_GREATER_GREATER},
    {"<<", TOKEN_ACTION_LESS_LESS},

};

TokenType identifierType(Scanner& scanner)
{
    std::string str(scanner.start, scanner.current - scanner.start);
    auto itr = identifiers.find(str);
    if (itr != identifiers.end())
    {
        return itr->second;
    }

    return TOKEN_IDENTIFIER;
}

Token identifier(Scanner& scanner)
{
    while (isAlpha(peek(scanner)) || isDigit(peek(scanner)))
    {
        advance(scanner);
    }
    return makeToken(scanner, identifierType(scanner));
}

Token number(Scanner& scanner)
{
    while (isDigit(peek(scanner)))
    {
        advance(scanner);
    }

    // Look for a fractional part.
    if (peek(scanner) == '.' && isDigit(peekNext(scanner)))
    {
        // Consume the ".".
        advance(scanner);

        while (isDigit(peek(scanner)))
        {
            advance(scanner);
        }
    }

    return makeToken(scanner, TOKEN_NUMBER);
}

Token string(Scanner& scanner, const char delim, TokenType type)
{
    while (peek(scanner) != delim && !isAtEnd(scanner))
    {
        if (peek(scanner) == '\n')
        {
            scanner.line++;
        }
        advance(scanner);
    }

    if (isAtEnd(scanner))
    {
        return errorToken(scanner, "Unterminated string.");
    }

    // The closing quote.
    advance(scanner);

    Token tok = makeToken(scanner, type);

    // Remove the quotes
    if (tok.length >= 2)
    {
        tok.start++;
        tok.length -= 2;
    }
    return tok;
}

} //namespace

void zixi_scanner_init(Scanner& scanner, const char* source)
{
    scanner.start = source;
    scanner.current = source;
    scanner.line = 1;
}

Token zixi_scanner_next_token(Scanner& scanner)
{
    skipWhitespace(scanner);
    scanner.start = scanner.current;

    if (isAtEnd(scanner))
    {
        return makeToken(scanner, TOKEN_EOF);
    }

    char c = advance(scanner);
    if (isAlpha(c))
    {
        return identifier(scanner);
    }

    if (isDigit(c))
    {
        return number(scanner);
    }

    switch (c)
    {
        case ':':
            return makeToken(scanner, TOKEN_COLON);
        case '(':
            return makeToken(scanner, TOKEN_LEFT_PAREN);
        case ')':
            return makeToken(scanner, TOKEN_RIGHT_PAREN);
        case ';':
            return makeToken(scanner, TOKEN_SEMICOLON);
        case '^':
            return makeToken(scanner, TOKEN_HAT);
        case ',':
            return makeToken(scanner, TOKEN_COMMA);
        case '.':
            return makeToken(scanner, TOKEN_DOT);
        case '-':
            return makeToken(scanner, match(scanner, '>') ? TOKEN_PRODUCES : TOKEN_MINUS);
        case '+':
            return makeToken(scanner, TOKEN_PLUS);
        case '/':
            return makeToken(scanner, TOKEN_SLASH);
        case '*':
            return makeToken(scanner, TOKEN_STAR);
        case '$':
            return makeToken(scanner, TOKEN_DOLLAR);
        case '!':
            return makeToken(scanner, match(scanner, '=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
        case '=':
            return makeToken(scanner, match(scanner, '=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
        case '<':
            return makeToken(scanner, match(scanner, '=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
        case '>':
            return makeToken(scanner, match(scanner, '=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
        case '"':
            return string(scanner, '"', TOKEN_STRING);
        case '|':
            return string(scanner, '|', TOKEN_PERCUSSIVE_PATTERN);
        case '{':
            return string(scanner, '}', TOKEN_CONCRETE_PATTERN);
        case '[':
            return string(scanner, ']', TOKEN_MELODIC_PATTERN);
    }

    return errorToken(scanner, "Unexpected character.");
}

const char* zixi_token_to_string(TokenType& tokenType)
{
    switch (tokenType)
    {
        case TOKEN_LEFT_PAREN:
            return "TOKEN_LEFT_PAREN";
        case TOKEN_RIGHT_PAREN:
            return "TOKEN_RIGHT_PAREN";
        case TOKEN_LEFT_BRACE:
            return "TOKEN_LEFT_BRACE";
        case TOKEN_RIGHT_BRACE:
            return "TOKEN_RIGHT_BRACE";
        case TOKEN_COMMA:
            return "TOKEN_COMMA";
        case TOKEN_DOT:
            return "TOKEN_DOT";
        case TOKEN_MINUS:
            return "TOKEN_MINUS";
        case TOKEN_PLUS:
            return "TOKEN_PLUS";
        case TOKEN_SEMICOLON:
            return "TOKEN_SEMICOLON";
        case TOKEN_SLASH:
            return "TOKEN_SLASH";
        case TOKEN_STAR:
            return "TOKEN_STAR";
        case TOKEN_BANG:
            return "TOKEN_BANG";
        case TOKEN_BANG_EQUAL:
            return "TOKEN_BANG_EQUAL";
        case TOKEN_EQUAL:
            return "TOKEN_EQUAL";
        case TOKEN_EQUAL_EQUAL:
            return "TOKEN_EQUAL_EQUAL";
        case TOKEN_GREATER:
            return "TOKEN_GREATER";
        case TOKEN_GREATER_EQUAL:
            return "TOKEN_GREATER_EQUAL";
        case TOKEN_LESS:
            return "TOKEN_LESS";
        case TOKEN_LESS_EQUAL:
            return "TOKEN_LESS_EQUAL";
        case TOKEN_IDENTIFIER:
            return "TOKEN_IDENTIFIER";
        case TOKEN_STRING:
            return "TOKEN_STRING";
        case TOKEN_NUMBER:
            return "TOKEN_NUMBER";
        case TOKEN_ERROR:
            return "TOKEN_ERROR";
        case TOKEN_EOF:
            return "TOKEN_EOF";
        case TOKEN_PERCUSSIVE_PATTERN:
            return "TOKEN_PERCUSSIVE_PATTERN";
        case TOKEN_MELODIC_PATTERN:
            return "TOKEN_MELODIC_PATTERN";
        case TOKEN_CONCRETE_PATTERN:
            return "TOKEN_CONCRETE_PATTERN";
        case TOKEN_ACTION_SHIFT:
            return "TOKEN_ACTION_SHIFT";
        case TOKEN_ACTION_GREATER_GREATER:
            return "TOKEN_ACTION_GREATER_GREATER";
        case TOKEN_ACTION_LESS_LESS:
            return "TOKEN_ACTION_LESS_LESS";
        case TOKEN_PRODUCES:
            return "TOKEN_PRODUCES";
        case TOKEN_COLON:
            return "TOKEN_COLON";
        case TOKEN_HAT:
            return "TOKEN_HAT";
        case TOKEN_DOLLAR:
            return "TOKEN_DOLLAR";
        case TOKEN_TUNING:
            return "TUNING";
        case TOKEN_TEMPO:
            return "TEMPO";
        case TOKEN_FUTURE:
            return "FUTURE";
        case TOKEN_SWAP:
            return "SWAP";
        case TOKEN_SHAKE:
            return "SHAKE";
        case TOKEN_DOZE:
            return "DOZE";
        case TOKEN_PERK:
            return "PERK";
        default:
            return "!UNKNOWN?";
    }
}

} //namespace Zixi
