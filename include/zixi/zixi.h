#include <string>
#include <vector>

namespace Zixi
{

struct TokenizerResult
{
    std::vector<std::string> tokens;
};

TokenizerResult zixi_tokenize(const std::string& input);

}