#ifndef COMMANDS_PARSER_H
#define COMMANDS_PARSER_H

#include <string>
#include <vector>

struct ParsedCommand
{
    std::string command;
    std::vector<std::string> args;
};

ParsedCommand parseCommand(const std::string& input);

#endif