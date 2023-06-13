#include <string>
#include <cassert>

#include <fmt/format.h>

#include <zest/logger/logger.h>
#include <zixi/parser.h>

namespace Zixi
{

namespace
{

std::string token_string(Token& tok)
{
    return std::string(tok.start, tok.length);
}

void consume(Parser& p)
{
    p.current = zixi_scanner_next_token(p.scanner);
}

void parse_declaration(Parser& p)
{
    // foo
    if (p.current.type == TOKEN_IDENTIFIER)
    {
        auto ident = token_string(p.current);
        consume(p);

        // foo ->
        if (p.current.type == TOKEN_PRODUCES)
        {
            Pattern pattern;
            pattern.name = ident;

            consume(p);

            if (p.current.type == TOKEN_IDENTIFIER)
            {
                pattern.synth = token_string(p.current);
                consume(p);
            }

            switch (p.current.type)
            {
                // foo -> [...]
                case TOKEN_MELODIC_PATTERN:
                    pattern.type = PatternType::Melodic;
                    pattern.values = token_string(p.current);
                    consume(p);
                    break;
                // foo -> {...}
                case TOKEN_CONCRETE_PATTERN:
                    pattern.type = PatternType::Concrete;
                    pattern.values = token_string(p.current);
                    consume(p);
                    break;
                // foo -> |...|
                case TOKEN_PERCUSSIVE_PATTERN:
                    pattern.type = PatternType::Percussive;
                    pattern.values = token_string(p.current);
                    consume(p);
                    break;
                default:
                    p.error = "Not a pattern";
                    return;
            }

            p.state.patterns[pattern.name] = pattern;
        }
        else if (p.current.type == TOKEN_ACTION_GREATER_GREATER)
        {
            consume(p);
            if (p.current.type == TOKEN_IDENTIFIER)
            {
                p.state.patterns[ident].name = ident;
                p.state.patterns[ident].pipes.push_back(token_string(p.current));
                consume(p);
            }
        }
    }
}

} //namespace

bool zixi_parse(Parser& p, const char* pszText)
{
    zixi_scanner_init(p.scanner, pszText);

    for (;;)
    {
        consume(p);
        if (p.current.type == TOKEN_EOF)
        {
            break;
        }

        if (p.current.type == TOKEN_ERROR)
        {
            p.error = token_string(p.current);
            break;
        }

        parse_declaration(p);

        if (!p.error.empty())
        {
            break;
        }
    }
    return p.error.empty();
}

std::string zixi_pattern_type_to_string(PatternType type)
{
    switch (type)
    {
        case PatternType::Concrete:
            return "Concrete";
        case PatternType::Melodic:
            return "Melodic";
        case PatternType::Percussive:
            return "Percussive";
        case PatternType::Unknown:
            return "Unknown";
        default:
            assert(!"Error");
            return "";
    }
}

std::string zixi_pattern_to_string(const Pattern& pattern)
{
    std::ostringstream str;
    str << "Name:" << pattern.name << ", Type:" << zixi_pattern_type_to_string(pattern.type) << ", Synth:" << pattern.synth << ", Values:'" << pattern.values << "'";

    bool effects = false;
    for (auto& pipe : pattern.pipes)
    {
        if (!effects)
        {
            effects = true;
            str << ", Effects: ";
        }
        str << " >> " << pipe;
    }
    return str.str();
}

} //namespace Zixi
