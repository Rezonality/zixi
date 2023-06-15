#pragma once

#include <unordered_map>
#include <set>
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

enum class ActionType
{
    Shake,
    Tempo,
    Doze,
    Perk,
    Swap,
    Transpose,
    Amplitude,
    Duration,
    AmplitudeUp,
    AmplitudeDown,
    Silence
};

enum class PeriodType
{
    Bar,
    Seconds
};

struct FutureTime
{
    PeriodType type = PeriodType::Seconds;
    int period = 0;
    int repeats = 1;
};

struct Modifier
{
    FutureTime future;
    ActionType type;
    std::string target;
};

struct Effect
{
    std::string name;
    bool add = true;
};

struct Pattern
{
    PatternType type = PatternType::Unknown;
    std::string name;
    std::string synth;
    std::string values;
    std::vector<Effect> effectChain;
};

struct IxiState
{
    std::unordered_map<std::string, Pattern> patterns;
    std::unordered_map<std::string, std::vector<Modifier>> patternModifiers;
    std::set<Modifier> modifiers;
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
