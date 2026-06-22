#include "commands_parser.h"

#include <sstream>

ParsedCommand parseCommand(const std::string& input)
{
    ParsedCommand result;

    std::stringstream ss(input);
    std::string token;

    bool first = true;

    while(ss >> token)
    {
        if(first)
        {
            result.command = token;
            first = false;
        }
        else
        {
            result.args.push_back(token);
        }
    }

    return result;
}