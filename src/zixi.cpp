#include <zest/logger/logger.h>
#include <zest/string/string_utils.h>

#include <zixi/zixi.h>
#include <zixi/scanner.h>

namespace Zest {
#undef ERROR
#ifdef _DEBUG
Logger logger = { true, LT::DBG };
#else
Logger logger = { true, LT::INFO };
#endif
bool Log::disabled = false;
}

using namespace Zest;

namespace Zixi
{

TokenizerResult zixi_tokenize(const std::string& input)
{
    TokenizerResult result;

    Scanner scanner;
    zixi_scanner_init(scanner, input.c_str());

    Token token;
    do
    {
        token = zixi_scanner_next_token(scanner);
        std::cout << "TOKEN: " << std::string(token.start, token.start + token.length) << " : " << zixi_token_to_string(token.type) << std::endl;

    } while (token.type != TokenType::TOKEN_EOF);

    return result;

}

}

