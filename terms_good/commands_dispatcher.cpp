#include "commands_dispatcher.h"
#include "commands.h"

#include <iostream>

void dispatchCommand(const ParsedCommand& cmd)
{
    if(cmd.command == "ls")
    {
        ls(cmd.args);
    }
    else if(cmd.command == "cd"){
        cd(cmd.args);
    }
    else if(cmd.command == "pwd"){
        pwd(cmd.args);
    }
    else if(cmd.command == "mkdir"){
        mkdir(cmd.args);
    }
    else if(cmd.command == "rm"){
        rm(cmd.args);
    }
    else if(cmd.command == "touch"){
        touch(cmd.args);
    }
    else if(cmd.command == "echo"){
        echo(cmd.args);
    }
    else if(cmd.command == "cat"){
        cat(cmd.args);
    }
    else if(cmd.command == "head"){
        head(cmd.args);
    }
    else if(cmd.command == "tail"){
        tail(cmd.args);
    }
    else if(cmd.command == "clear"){
        clear();
    }
    else if(cmd.command == "mv"){
        mv(cmd.args);
    }
    else if(cmd.command == "diff"){
        diff(cmd.args);
    }
    else if(cmd.command == "cp"){
        cp(cmd.args);
    }
    else if(cmd.command == "help"){
        help(cmd.args);
    }
    else if(cmd.command == "stat"){
        stat(cmd.args);
    }
    else if(cmd.command == "find"){
        find(cmd.args);
    }
    else if(cmd.command == "grep"){
        grep(cmd.args);
    }
    else if(cmd.command == "wc"){
        wc(cmd.args);
    }
    else if(cmd.command == "history")
    {
        history_command(cmd.args);
    }
    else if(cmd.command == "tree"){
        tree(cmd.args);
    }
    else if(cmd.command == "search"){
        search(cmd.args);
    }
    else if(cmd.command == "neofetch"){
        neofetch(cmd.args);
    }
    else
    {
        std::cout
            << "Unknown command: "
            << cmd.command
            << std::endl;
    }
}