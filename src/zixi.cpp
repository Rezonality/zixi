#include <zest/logger/logger.h>
#include <zest/string/string_utils.h>

#include <zixi/zixi.h>

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
    result.tokens = string_split(input, " ");
    return result;

}

}

