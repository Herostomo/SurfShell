#pragma once
#ifndef COMMANDS_H
#define COMMANDS_H
#include <vector>
#include <string>

// Basic File Commands
void ls(const std::vector<std::string>& args); // List directory contents
void cp(const std::vector<std::string>& args);// Copy files and directories
void mv(const std::vector<std::string>& args);  // Move or rename files and directories
void rm(const std::vector<std::string>& args);// Remove files and directories
void pwd(const std::vector<std::string>& args);// Print working directory

// Directory Management
void cd(const std::vector<std::string>& args);// Change directory
void mkdir(const std::vector<std::string>& args);// Create directories

// File Viewing
void cat(const std::vector<std::string>& args);// Concatenate and display file contents
void head(const std::vector<std::string>& args);// Display the first few lines of a file
void tail(const std::vector<std::string>& args);// Display the last few lines of a file

// File Properties
void touch(const std::vector<std::string>& args);// Create an empty file or update the timestamp of an existing file
void stat(const std::vector<std::string>& args);// Display file or directory status
void echo(const std::vector<std::string>& args);//to read and write the files
// Searching
void find(const std::vector<std::string>& args);// Search for files in a directory hierarchy

// File Manipulation
void grep(const std::vector<std::string>& args);// Search for patterns in files
// void sort_cmd(const std::vector<std::string>& args);// Sort lines of text files
// void uniq(const std::vector<std::string>& args);// Report or omit repeated lines in a file

// Disk Usage
// void df(const std::vector<std::string>& args);// Report file system disk space usage
void diff(const std::vector<std::string>& args);// Compare files line by line

// System Commands
// void cpu(const std::vector<std::string>& args);// Display CPU usage
// void ram(const std::vector<std::string>& args);// Display RAM usage
// void disk(const std::vector<std::string>& args);// Display disk usage
// void processes(const std::vector<std::string>& args);// Display running processes
// void kill_cmd(const std::vector<std::string>& args);// Terminate a process by PID
void wc(const std::vector<std::string>& args);
// Shell Commands
void help();// Display help information for commands
void clear();// Clear the terminal screen
// void history(const std::vector<std::string>& args);// Display command history
// void exit_cmd(const std::vector<std::string>& args);// Exit the shell
void history_command(const std::vector<std::string>& args);
void tree(const std::vector<std::string>& args);

struct SearchResult
{
    std::string title;
    std::string url;
};

void search(const std::vector<std::string>& args);
void neofetch(const std::vector<std::string>& args);
#endif