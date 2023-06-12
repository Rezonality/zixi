#include <cstdio>
#include <cstring>

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

TokenType checkKeyword(Scanner& scanner, int start, int length, const char* rest, TokenType type)
{
    if (scanner.current - scanner.start == start + length && memcmp(scanner.start + start, rest, length) == 0)
    {
        return type;
    }

    return TOKEN_IDENTIFIER;
}

TokenType identifierType(Scanner& scanner)
{
    //> keywords
    switch (scanner.start[0])
    {
        case '>':
            if ((scanner.current - scanner.start) >= 1)
            {
                switch (scanner.start[1])
                {
                    case '>':
                        scanner.current++;
                        return TOKEN_ACTION_FORWARDS;
                    case 's':
                        return checkKeyword(scanner, 2, 4, "hift", TOKEN_ACTION_GREATER_SHIFT);
                }
            }
            break;
        case 'a':
            return checkKeyword(scanner, 1, 2, "nd", TOKEN_AND);
        case 'c':
            return checkKeyword(scanner, 1, 4, "lass", TOKEN_CLASS);
        case 'e':
            return checkKeyword(scanner, 1, 3, "lse", TOKEN_ELSE);
        case 'f':
            if (scanner.current - scanner.start > 1)
            {
                switch (scanner.start[1])
                {
                    case 'a':
                        return checkKeyword(scanner, 2, 3, "lse", TOKEN_FALSE);
                    case 'o':
                        return checkKeyword(scanner, 2, 1, "r", TOKEN_FOR);
                    case 'u':
                        return checkKeyword(scanner, 2, 1, "n", TOKEN_FUN);
                }
            }
            break;
        case 'i':
            return checkKeyword(scanner, 1, 1, "f", TOKEN_IF);
        case 'n':
            return checkKeyword(scanner, 1, 2, "il", TOKEN_NIL);
        case 'o':
            return checkKeyword(scanner, 1, 1, "r", TOKEN_OR);
        case 'p':
            return checkKeyword(scanner, 1, 4, "rint", TOKEN_PRINT);
        case 'r':
            return checkKeyword(scanner, 1, 5, "eturn", TOKEN_RETURN);
        case 's':
            return checkKeyword(scanner, 1, 4, "uper", TOKEN_SUPER);
        case 't':
            if (scanner.current - scanner.start > 1)
            {
                switch (scanner.start[1])
                {
                    case 'h':
                        return checkKeyword(scanner, 2, 2, "is", TOKEN_THIS);
                    case 'r':
                        return checkKeyword(scanner, 2, 2, "ue", TOKEN_TRUE);
                }
            }
            break;
        case 'v':
            return checkKeyword(scanner, 1, 2, "ar", TOKEN_VAR);
        case 'w':
            return checkKeyword(scanner, 1, 4, "hile", TOKEN_WHILE);
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
    return makeToken(scanner, type);
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
    if (isAlpha(c) || c == '>')
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
        case TOKEN_AND:
            return "TOKEN_AND";
        case TOKEN_CLASS:
            return "TOKEN_CLASS";
        case TOKEN_ELSE:
            return "TOKEN_ELSE";
        case TOKEN_FALSE:
            return "TOKEN_FALSE";
        case TOKEN_FOR:
            return "TOKEN_FOR";
        case TOKEN_FUN:
            return "TOKEN_FUN";
        case TOKEN_IF:
            return "TOKEN_IF";
        case TOKEN_NIL:
            return "TOKEN_NIL";
        case TOKEN_OR:
            return "TOKEN_OR";
        case TOKEN_PRINT:
            return "TOKEN_PRINT";
        case TOKEN_RETURN:
            return "TOKEN_RETURN";
        case TOKEN_SUPER:
            return "TOKEN_SUPER";
        case TOKEN_THIS:
            return "TOKEN_THIS";
        case TOKEN_TRUE:
            return "TOKEN_TRUE";
        case TOKEN_VAR:
            return "TOKEN_VAR";
        case TOKEN_WHILE:
            return "TOKEN_WHILE";
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
        case TOKEN_ACTION_GREATER_SHIFT:
            return "TOKEN_ACTION_GREATER_SHIFT";
        case TOKEN_ACTION_FORWARDS:
            return "TOKEN_ACTION_FORWARDS";
        case TOKEN_PRODUCES:
            return "TOKEN_PRODUCES";
        case TOKEN_COLON:
            return "TOKEN_COLON";
        case TOKEN_HAT:
            return "TOKEN_HAT";
        case TOKEN_DOLLAR:
            return "TOKEN_DOLLAR";
        default:
            return "!UNKNOWN?";
    }
}

} //namespace Zixi
