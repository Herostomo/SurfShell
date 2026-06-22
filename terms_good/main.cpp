#include <iostream>
#include <string>
#include <windows.h>
#include "commands_parser.h"
#include <filesystem>
#include "commands_dispatcher.h"
std::vector<std::string> command_history;



int main()
{

    std::string input;

    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    DWORD dwMode = 0;

    GetConsoleMode(hOut, &dwMode);

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

    SetConsoleMode(hOut, dwMode);
    std::cout << "\033[2J\033[H";

    std::cout << R"(

    ██╗  ██╗████████╗███████╗██████╗ ███╗   ███╗
    ██║ ██╔╝╚══██╔══╝██╔════╝██╔══██╗████╗ ████║
    █████╔╝    ██║   █████╗  ██████╔╝██╔████╔██║
    ██╔═██╗    ██║   ██╔══╝  ██╔══██╗██║╚██╔╝██║
    ██║  ██╗   ██║   ███████╗██║  ██║██║ ╚═╝ ██║
    ╚═╝  ╚═╝   ╚═╝   ╚══════╝╚═╝  ╚═╝╚═╝     ╚═╝

    )";

    std::cout
    << "\033[1;36m"
    << "Kshitij Terminal v1.0\n"
    << "\033[0m";

    std::cout
    << "Type "
    << "\033[1;32mhelp\033[0m"
    << " for commands.\n\n";
    while(true)
    {
        std::string cwd = std::filesystem::current_path().string();

        std::cout
            << "\033[1;36m"
            << "╭─["
            << "\033[1;32m"
            << "kshitij@arch"
            << "\033[1;36m"
            << "]\n"
            << "╰─["
            << "\033[1;34m"
            << cwd
            << "\033[1;36m"
            << "] "
            << "\033[1;35m"
            << "❯ "
            << "\033[0m";

        std::getline(std::cin, input);
        
        if(input.empty())
        {
            continue;
        }
        if(input != "history")
        {
             command_history.push_back(input);
        }
        if(input == "exit")
        {
            break;
        }

        ParsedCommand cmd = parseCommand(input);

        dispatchCommand(cmd);
    }

    return 0;
}