#pragma once

#include <unordered_map>
#include <zixi/scanner.h>

namespace Zixi
{

enum class PatternType
{
    Melodic,
    Percussive,
    Concrete,
    Unknown
};

struct Pattern
{
    PatternType type = PatternType::Unknown;
    std::string name;
    std::string synth;
    std::string values;
    std::vector<std::string> pipes;
};

struct IxiState
{
    std::unordered_map<std::string, Pattern> patterns;
};

struct Parser
{
    Scanner scanner;
    Token current;
    std::string error;
    IxiState state;
};

bool zixi_parse(Parser& parser, const char* pszText);
std::string zixi_pattern_type_to_string(PatternType type);
std::string zixi_pattern_to_string(const Pattern& type);

} //namespace Zixi
