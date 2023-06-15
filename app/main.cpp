#pragma warning(disable : 4005)
#include "pch.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

#include <zest/file/file.h>
#include <zest/logger/logger.h>
#include <zest/string/string_utils.h>

#include <zixi/parser.h>
#include <zixi/zixi.h>

using namespace Zixi;

void out(const std::string& val = std::string())
{
    std::cout << val << std::endl;
}

void update_state(IxiState& state, IxiState newState)
{
    // Add newly declared patterns
    for (auto& [name, pattern] : newState.patterns)
    {
        if (state.patterns.find(name) == state.patterns.end())
        {
            state.patterns[name] = pattern;
        }
    }

    for (auto& [name, pattern] : state.patterns)
    {
        // Already have this pattern?
        if (newState.patterns.find(name) != newState.patterns.end())
        {
            for (auto& effect : pattern.effectChain)
            {
                if (effect.name.empty())
                {
                    pattern.effectChain.clear();
                }
                else
                {
                    // Add or remove on the effect chain
                    if (effect.add)
                    {
                        pattern.effectChain.push_back(effect);
                    }
                    else
                    {
                        // Remove all of the same type?
                        std::remove_if(pattern.effectChain.begin(), pattern.effectChain.end(), [&](auto& val) {
                            return (val.name == effect.name);
                        });
                    }
                }
            }
        }
    }
}

int main(int argc, char* argv[])
{
    std::string input;
    if (argc == 2)
    {
        input = Zest::file_read(argv[1]);
    }
    else
    {
        IxiState ixi;

        std::string line;
        while (std::getline(std::cin, line))
        {

            // Exit the loop if a specific condition is met
            if (line == "quit")
            {
                break;
            }

            Parser parser;
            zixi_parse(parser, line.c_str());

            out();
            if (!parser.error.empty())
            {
                out(parser.error);
                continue;
            }

            update_state(ixi, parser.state);

            out("Parser Output : ");
            for (auto& [name, pat] : ixi.patterns)
            {
                out(zixi_pattern_to_string(pat));
            }
            out("(END)");
            out();
        }
    }
}